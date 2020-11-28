 #include "phenoselector.h"

PhenoSelector::PhenoSelector()
{

}

/**
 * @brief PhenoSelector::PhenoSelector      constructor function
 * @param selectedPheno
 * @param excludedPheno
 */
PhenoSelector::PhenoSelector(QStringList const selectedPheno, QStringList const excludedPheno)
{
    if (!selectedPheno.isEmpty())
    {
        this->selectedPhenoList = selectedPheno;
    }

    if (!excludedPheno.isEmpty())
    {
        this->excludedPhenoList = excludedPheno;
    }
}

/**
 * @brief PhenoSelector::setSelectedPheno
 * @param phenoList  selected phenotype list.
 */
void PhenoSelector::setSelectedPheno(QStringList const phenoList)
{
    if (!phenoList.isEmpty())
    {
        this->selectedPhenoList = phenoList;
    }
    else
    {
        this->selectedPhenoList.clear();
    }
}

/**
 * @brief PhenoSelector::getSelectedPheno
 * @return selected phenotype list.
 */
QStringList PhenoSelector::getSelectedPheno(void)
{
    return this->selectedPhenoList;
}

/**
 * @brief PhenoSelector::setExcludedPheno
 * @param phenoList  Excluded phenotype list.
 */
void PhenoSelector::setExcludedPheno(QStringList const phenoList)
{
    if (!phenoList.isEmpty())
    {
        this->excludedPhenoList = phenoList;
    }
    else
    {
        this->excludedPhenoList.clear();
    }
}

/**
 * @brief PhenoSelector::getExcludedPheno
 * @return  Excluded phenotype list.
 */
QStringList PhenoSelector::getExcludedPheno(void)
{
    return this->excludedPhenoList;
}

/**
 * @brief PhenoSelector::selectPheno
 * @param phenoItems    selected phenotype list.
 */
void PhenoSelector::selectPheno(QList<QListWidgetItem *> phenoItems)
{
    if (!phenoItems.isEmpty())
    {
        for (auto item : phenoItems)
        {   // Move current item from selected list to excluded list.
            this->selectedPhenoList.append(item->text());
            this->excludedPhenoList.removeOne(item->text());
        }
    }
}

/**
 * @brief PhenoSelector::excludePheno
 * @param phenoItems    exclued phenotype list.
 */
void PhenoSelector::excludePheno(QList<QListWidgetItem *> phenoItems)
{
    if (!phenoItems.isEmpty())
    {
        for (auto item : phenoItems)
        {   // Move current from excluded list to selected list.
            this->selectedPhenoList.removeOne(item->text());
            this->excludedPhenoList.append(item->text());
        }
    }
}

void PhenoSelector::selectAllPheno(void)
{
    if (!this->excludedPhenoList.isEmpty())
    {   // Move all item from selected list to excluded list.
        this->selectedPhenoList.append(excludedPhenoList);
        this->excludedPhenoList.clear();
    }
}

void PhenoSelector::excludeAllPheno(void)
{
    if (!this->selectedPhenoList.isEmpty())
    {   // Move all from excluded list to selected list.
        this->excludedPhenoList.append(this->selectedPhenoList);
        this->selectedPhenoList.clear();
    }
}
