<?php

class EntidadePar extends Modelo {

    public function carregarDadosMunicipioEstado($esfera, $cod, $arrDutid, $coluna) {
        $strWhere = "";
        if ($esfera == "M") {
            $strWhere = " AND ent.muncod = '$cod'";
        } else {
            $strWhere = " AND ent.estuf = '$cod'";
        }

        $sql = "
                SELECT
                    upper(REPLACE({$coluna},'\'','')) as municipio,
                    ent.estuf,
                    ent.entnome as prefeitura,
                    coalesce(ent.endlog,'#') || '/' || coalesce(ent.endbai,'#') as prefendereco,
                    ent.entnumcpfcnpj as cnpjprefeitura,
                    ent2.entnome as prefeito,
                    ent2.entnumrg as rg,
                    ent2.entnumcpfcnpj as cpf,
                    est.estdescricao as estado
                FROM 
                    par.entidade ent 
                INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = {$arrDutid['ent2']} AND ent2.entstatus = 'A'
                INNER JOIN territorios.municipio mun ON mun.muncod = ent.muncod
                INNER JOIN territorios.estado est ON est.estuf = ent.estuf
                WHERE
                    ent.dutid = {$arrDutid['ent']} 
                    AND ent.entstatus = 'A' 
                    {$strWhere}";
        return $this->carregar($sql);
    }

}
