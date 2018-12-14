/****************************************************************************
* GRAVITATE Desktop Client                                                  *
*                                                                           *
* Consiglio Nazionale delle Ricerche                                        *
* Istituto di Matematica Applicata e Tecnologie Informatiche                *
* Sezione di Genova                                                         *
* IMATI-GE / CNR                                                            *
*                                                                           *
* Authors: Andrea Repetto                                                   *
* Copyright(C) 2018: IMATI-GE / CNR                                         *
* All rights reserved.                                                      *
*                                                                           *
* This program is dual-licensed as follows:                                 *
*                                                                           *
* (1) You may use GRAVITATE Desktop Client as free software; you can        *
* redistribute it and/or modify it under the terms of the GNU General       *
* Public License as published by the Free Software Foundation; either       *
* version 3 of the License, or (at your option) any later  version.         *
* In this case the program is distributed in the hope that it will be       *
* useful, but WITHOUT ANY WARRANTY; without even the implied warranty of    *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
* (2) You may use GRAVITATE Desktop Client as part of a commercial          *
* software. In this case a proper agreement must be reached with the        *
* Authors and with IMATI-GE/CNR based on a proper licensing contract.       *
*                                                                           *
****************************************************************************/
#ifndef QHASHTREE_H
#define QHASHTREE_H

#include <QHash>
#include <QList>
#include <stdexcept>
#include <algorithm>

/**
 * This class is a tree data structure, where each node can have a variable
 * number of children.
 *
 * It is specifically designed for use with QAbstractItemModel.
 *
 */
template<typename T>
class QHashTree
{
    struct Node
    {
        T value;
        quintptr parent;
        QList<quintptr> children;
    };

public:

    static const quintptr NULL_NODE = 0;

    QHashTree() {
        clear();
    }

    bool isEmpty()
    {
        return m_nodes.isEmpty();
    }

    bool contains(quintptr id) const
    {
        return m_nodes.contains(id);
    }

    quintptr rootId() const
    {
        return m_rootId;
    }

    quintptr insertRoot(const T &node)
    {
        if(m_rootId != NULL_NODE)
            throw std::runtime_error("Root already inserted in the tree");

        m_rootId = insert(node, NULL_NODE);
        return m_rootId;
    }

    quintptr insert(const T &node, quintptr parentId)
    {
        quintptr id = m_nextNodeId;

        Node n;
        n.value = node;
        n.parent = parentId;
        if(parentId != NULL_NODE)
        {
            m_nodes[parentId].children.append(id);
        }

        m_nodes.insert(id, n);

        m_nextNodeId++;
        return id;
    }

    quintptr insertAt(const T &node, int pos, quintptr parentId)
    {
        quintptr id = m_nextNodeId;

        Node n;
        n.value = node;
        n.parent = parentId;
        if(parentId != NULL_NODE)
        {
            m_nodes[parentId].children.insert(pos, id);
        }

        m_nodes.insert(id, n);

        m_nextNodeId++;
        return id;
    }


    const T getNode(quintptr id) const
    {
        return m_nodes.value(id).value;
    }

    const quintptr child(quintptr nodeId, int idx) const
    {
        if(nodeId == NULL_NODE)
            nodeId = rootId();
        Node node = m_nodes.value(nodeId);
        return node.children.at(idx);
    }

    const int childCount(quintptr id) const
    {
        Node node = m_nodes.value(id);
        return node.children.count();
    }

    const int row(quintptr id) const
    {
        Node node = m_nodes.value(id);
        Node parent = m_nodes.value(node.parent);
        return parent.children.indexOf(id);
    }

    const quintptr parent(quintptr id) const
    {
        Node node = m_nodes.value(id);
        return node.parent;
    }

    void clear()
    {
        m_nodes.clear();
        m_rootId = NULL_NODE;
        m_nextNodeId = 1;
    }

    /**
     * @brief remove: It removes a node and all its children
     * @param nodeId the ID of the node to be deleted
     *
     */
    void remove(quintptr nodeId)
    {
        for(int i = 0; i < childCount(nodeId); i++)
        {
            quintptr childId = child(nodeId, i);
            remove(childId);
        }
        int rowInParent = row(nodeId);
        quintptr parentId = parent(nodeId);

        if(m_nodes.contains(parentId))
        {
            m_nodes[parentId].children.removeAt(rowInParent);
        }
        m_nodes.remove(nodeId);
    }

    template<class Compare>
    void sortChildren(quintptr nodeId, Compare comp)
    {
        auto node = m_nodes.value(nodeId);
        std::stable_sort(node.children.begin(), node.children.end(),
        [this, comp] (quintptr p1, quintptr p2)
        {
            auto n1 = getNode(p1);
            auto n2 = getNode(p2);
            return comp(n1, n2);
        });
    }

private:

    QHash<quintptr, Node> m_nodes;
    quintptr m_nextNodeId = 1;
    quintptr m_rootId;

};

#endif // QHASHTREE_H
