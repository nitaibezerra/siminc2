<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of Snapshot
 *
 * @author LindalbertoFilho
 */
class Spo_Service_Periodo
{
    public function __construct()
    {

    }

    public function listaPeriodoAcao()
    {
        $acomporc = new Acomporc_Model_PeriodoReferencia();
        $colunas = "
            prftitulo,
            date_part('month',prfinicio) as mes_inicio,
            date_part('month',prffim) as mes_fim,
            date_part('day',prfinicio) as dia_inicio,
            date_part('day',prffim) as dia_fim,
            TO_CHAR(prfinicio, 'DD/MM/YYYY') as prfinicio,
            TO_CHAR(prffim, 'DD/MM/YYYY') as prffim,
            TO_CHAR(prfpreenchimentoinicio, 'DD/MM/YYYY') as prfpreenchimentoinicio,
            TO_CHAR(prfpreenchimentofim, 'DD/MM/YYYY') as prfpreenchimentofim";
        $lista = $acomporc->lista($colunas, array("prftipo = 'A'","prsano = '{$_SESSION['exercicio']}'"), null, array('order' => 'prfid ASC'));
        return $lista ? $lista : array();
    }

    public function listaPeriodoSubacao()
    {
        $acomporc = new Acomporc_Model_PeriodoReferencia();
        $colunas = "
            prftitulo,
            date_part('month',prfinicio) as mes_inicio,
            date_part('month',prffim) as mes_fim,
            date_part('day',prfinicio) as dia_inicio,
            date_part('day',prffim) as dia_fim,
            TO_CHAR(prfinicio, 'DD/MM/YYYY') as prfinicio,
            TO_CHAR(prffim, 'DD/MM/YYYY') as prffim,
            TO_CHAR(prfpreenchimentoinicio, 'DD/MM/YYYY') as prfpreenchimentoinicio,
            TO_CHAR(prfpreenchimentofim, 'DD/MM/YYYY') as prfpreenchimentofim";
        $lista = $acomporc->lista($colunas, array("prftipo = 'S'","prsano = '{$_SESSION['exercicio']}'"), null, array('order' => 'prfid ASC'));
        return $lista ? $lista : array();
    }

    public function listaPeriodoRelatorioTCU()
    {
        $acomporc = new Acomporc_Model_PeriodoReferencia();
        $colunas = "
            prftitulo,
            date_part('month',prfinicio) as mes_inicio,
            date_part('month',prffim) as mes_fim,
            date_part('day',prfinicio) as dia_inicio,
            date_part('day',prffim) as dia_fim,
            TO_CHAR(prfinicio, 'DD/MM/YYYY') as prfinicio,
            TO_CHAR(prffim, 'DD/MM/YYYY') as prffim,
            TO_CHAR(prfpreenchimentoinicio, 'DD/MM/YYYY') as prfpreenchimentoinicio,
            TO_CHAR(prfpreenchimentofim, 'DD/MM/YYYY') as prfpreenchimentofim";
        $lista = $acomporc->lista($colunas, array("prftipo = 'T'","prsano = '{$_SESSION['exercicio']}'"), null, array('order' => 'prfid ASC'));
        return $lista ? $lista : array();
    }

    public function listaPeriodoReceitaOrcamentaria()
    {
        global $db;
        $query = <<<DML
            SELECT
                prfdsc as prftitulo,
                date_part('month',prfdatainicio) as mes_inicio,
                date_part('month',prfdatafim) as mes_fim,
                date_part('day',prfdatainicio) as dia_inicio,
                date_part('day',prfdatafim) as dia_fim,
                TO_CHAR(prfdatainicio, 'DD/MM/YYYY') as prfinicio,
                TO_CHAR(prfdatafim, 'DD/MM/YYYY') as prffim,
                TO_CHAR(prfpreenchimentoinicio, 'DD/MM/YYYY') as prfpreenchimentoinicio,
                TO_CHAR(prfpreenchimentofim, 'DD/MM/YYYY') as prfpreenchimentofim
            FROM recorc.periodoreferencia
	    WHERE (prfstatus = 'A') AND (exercicio = '{$_SESSION['exercicio']}')
            ORDER BY prfid ASC;
DML;
        $lista = $db->carregar($query);
        return $lista ? $lista : array();
    }

    public function listaPeriodoProposta()
    {
        global $db;
        $query = <<<DML
            SELECT
                prftitulo,
                date_part('month',prfinicio) as mes_inicio,
                date_part('month',prffim) as mes_fim,
                date_part('day',prfinicio) as dia_inicio,
                date_part('day',prffim) as dia_fim,
                TO_CHAR(prfinicio, 'DD/MM/YYYY') as prfinicio,
                TO_CHAR(prffim, 'DD/MM/YYYY') as prffim,
                TO_CHAR(prfpreenchimentoinicio, 'DD/MM/YYYY') as prfpreenchimentoinicio,
                TO_CHAR(prfpreenchimentofim, 'DD/MM/YYYY') as prfpreenchimentofim
            FROM proporc.periodoreferencia
            WHERE (prsano = '{$_SESSION['exercicio']}')
            ORDER BY prfid ASC;
DML;
        $lista = $db->carregar($query);
        return $lista ? $lista : array();
    }

    public function listaPeriodoAlteracao()
    {
        global $db;
        $query = <<<DML
            SELECT
                mcrdsc as prftitulo,
                date_part('month',mcrrefinicio) as mes_inicio,
                date_part('month',mcrreffim) as mes_fim,
                date_part('day',mcrrefinicio) as dia_inicio,
                date_part('day',mcrreffim) as dia_fim,
                TO_CHAR(mcrrefinicio, 'DD/MM/YYYY') as prfinicio,
                TO_CHAR(mcrreffim, 'DD/MM/YYYY') as prffim,
                TO_CHAR(mcrincinicio, 'DD/MM/YYYY') as prfpreenchimentoinicio,
                TO_CHAR(mcrincfim, 'DD/MM/YYYY') as prfpreenchimentofim
            FROM altorc.momentocredito
            WHERE (mcrano = '{$_SESSION['exercicio']}')
            ORDER BY mcrid ASC;
DML;
        $lista = $db->carregar($query);
        return $lista ? $lista : array();
    }


}
