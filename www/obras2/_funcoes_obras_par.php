<?php

include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . "includes/library/simec/Grafico.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/ChecklistFnde.class.inc";

/**
 * @deprecated
 * @param $obras
 * @return array
 */
function agrupaObras($obras)
{
    $final = array();

    foreach($obras as $key => $obra){

        if(!is_array($obra['situacao']))
            $obra['situacao'] = array($obra['situacao']);

        if(isset($final[$obra['obrid']])){
                $final[$obra['obrid']]['situacao'] = array_merge($obra['situacao'], $final[$obra['obrid']]['situacao']);
        } else {
            $final[$obra['obrid']] = $obra;
        }

    }

    foreach ($final as $key => $obra) {
        $final[$key]['situacao'] = implode('<br />' ,$final[$key]['situacao']);
    }

    return $final;
}
/**
 * @deprecated
 * @param $aMunicipio
 * @return array|void
 */
function getObrasMunicipio($aMunicipio){
    global $db;
    $sqlObras = " SELECT
                        o.obrid,
                        pre.preid,
                                case
                                        when pre.predescricao <> '' then pre.predescricao
                                        else o.obrnome
                                end as predescricao,
                        m.estuf,
                        m.mundescricao as descricao,
                        m.muncod,
                        'Obra com restrição gerada pelo checklist.' as situacao,
                        null as pagvalorparcela,
                        coalesce(o.obrpercentultvistoria, 0) as obrpercentultvistoria
                FROM obras2.obras o
                        LEFT JOIN obras.preobra pre ON o.obrid = pre.obrid
                        INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
                        INNER JOIN workflow.documento d ON d.docid = o.docid
                        LEFT JOIN  entidade.endereco ed on ed.endid = o.endid
                        LEFT JOIN  territorios.municipio m on m.muncod = ed.muncod
                WHERE o.obrstatus = 'A'
                AND e.orgid = 3
                AND e.empesfera = 'M'
                AND o.obridpai IS NULL
                AND m.muncod in ('" . implode("', '", $aMunicipio) . "')
                ";
    $result = $db->carregar($sqlObras);
    return ($result) ? $result : array();
}
/**
 * @deprecated
 * @param $estuf
 * @return array|void
 */
function getObrasEstado($estuf){
    global $db;

    if($estuf){
        $estuf = (array) $estuf;

        $sql = "
                    SELECT
                        o.obrid,
                        pre.preid,
                                case
                                        when pre.predescricao <> '' then pre.predescricao
                                        else o.obrnome
                                end as predescricao,
                        m.estuf,
                        m.mundescricao as descricao,
                        m.muncod,
                        'Obra com problemas no checklist.' as situacao,
                        NULL AS pagvalorparcela,
                        coalesce(o.obrpercentultvistoria, 0) as obrpercentultvistoria
                FROM obras2.obras o
                        LEFT JOIN obras.preobra pre ON o.obrid = pre.obrid
                        INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
                        INNER JOIN workflow.documento d ON d.docid = o.docid
                        LEFT JOIN  entidade.endereco ed on ed.endid = o.endid
                        LEFT JOIN  territorios.municipio m on m.muncod = ed.muncod
                WHERE o.obrstatus = 'A'
                AND e.orgid = 3
                AND e.empesfera = 'E'
                AND o.obridpai IS NULL
                AND m.estuf in ('" . implode("', '", $estuf) . "')
                ";
        $result = $db->carregar($sql);
        return ($result) ? $result : array();
    }
    return array();
}

/**
 * Recupera obras com pendências no checklist pelo CPF
 * Obras com restrição/inconformidade gerada pelo checklista que ainda não foi superada
 *
 * @param string $cpf
 * @return array - Obras pendentes
 */
function getObrasPendentesPreenchimentoPorCpf($cpf){
    $arEmpid 	 = pegaEmpidPermitido($cpf);

    $empreendimento = new Empreendimento();
    $municipios = $empreendimento->pegaMuncodPorEmpid($arEmpid);

    $esfera = verificaEsferaEmpreendimento($arEmpid);

    $aMunicipio = array();
    $aEstado 	= array();
    if(is_array($municipios)){
        if( $esfera == 'E' ){
            foreach($municipios as $municipio){
                $aEstado[] = $municipio['estuf'];
            }
            return getObrasPendentesPreenchimentoPorEstado($aEstado);
        } else {
            foreach($municipios as $municipio){
                $aMunicipio[] = $municipio['muncod'];
            }
            return getObrasPendentesPreenchimentoPorMunicipio($aMunicipio);
        }
    }

}

/**
 * Recupera obras com pendências no checklist
 * Obras com restrição/inconformidade gerada pelo checklista que ainda não foi superada
 *
 * @param string $muncod
 * @return array - Obras pendentes
 */
function getObrasPendentesPreenchimentoPorMunicipio($aMunicipio){
    global $db;

    if($aMunicipio){
        $aMunicipio = (array) $aMunicipio;


        $sql = "
               SELECT
                *
                FROM obras2.vm_total_pendencias
                WHERE
                empesfera = 'M'
                AND muncod in ('" . implode("', '", $aMunicipio) . "')
                AND pendencia = 'preenchimento'
                ";

        $result = $db->carregar($sql);
        $result = ($result) ? $result : array();
        return $result;
    }
    return array();
}

/**
 * Recupera obras com pendências no checklist
 * Obras com restrição/inconformidade gerada pelo checklista que ainda não foi superada
 *
 * @param string $estuf
 * @return array - Obras pendentes
 */
function getObrasPendentesPreenchimentoPorEstado($estuf){
    global $db;

    if($estuf){
        $estuf = (array) $estuf;


        $sql = "
                SELECT
                *
                FROM obras2.vm_total_pendencias
                WHERE
                empesfera = 'E'
                AND pendencia = 'preenchimento'
                AND estuf in ('" . implode("', '", $estuf) . "')
                ";

        $result = $db->carregar($sql);
        $result = ($result) ? $result : array();
        return $result;

    }
    return array();
}

/**
 * Recupera obras com pendências para novos recursos
 * Obras com percentual de evolução < 10%
 * Obras com restrição nao superada
 *
 * @param string $estuf
 * @return array - Obras pendentes
 */
function getObrasPendentesDesenbolsoPorEstado($estuf){
    global $db;

    if($estuf){
        $estuf = (array) $estuf;

        $sql = "
                SELECT
                *
                FROM obras2.vm_total_pendencias
                WHERE
                empesfera = 'E'
                AND estuf in ('" . implode("', '", $estuf) . "')
                AND pendencia = 'desembolso'
                ";
        $result = $db->carregar($sql);
        $result = ($result) ? $result : array();
        return $result;

    }
    return array();
}

/**
 * Recupera obras com pendências para novos recursos
 * Obras com percentual de evolução < 10%
 * Obras com restrição nao superada
 *
 * @param string $muncod
 * @return array - Obras pendentes
 */
function getObrasPendentesDesenbolsoPorMunicipio($aMunicipio){
    global $db;

    if($aMunicipio){
        $aMunicipio = (array) $aMunicipio;

        $sql = "
                   SELECT
                *
                FROM obras2.vm_total_pendencias
                WHERE
                empesfera = 'M'
                AND muncod in ('" . implode("', '", $aMunicipio) . "')
                AND pendencia = 'desembolso'
                ";
        $result = $db->carregar($sql);
        $result = ($result) ? $result : array();
        return $result;
    }
    return array();
}

/**
 * Recupera obras pendentes de novos recursos por CPF vinculado:
 * Obras com o percentual de evolução menor que 10%
 *
 * @param string $cpf
 * @return array - Obras pendentes
 */
function getObrasPendentesDesenbolsoPorCpf($cpf){
    $arEmpid 	 = pegaEmpidPermitido($cpf);

    $empreendimento = new Empreendimento();
    $municipios = $empreendimento->pegaMuncodPorEmpid($arEmpid);

    $esfera = verificaEsferaEmpreendimento($arEmpid);

    $aMunicipio = array();
    $aEstado 	= array();
    if(is_array($municipios)){
        if( $esfera == 'E' ){
            foreach($municipios as $municipio){
                $aEstado[] = $municipio['estuf'];
            }
            return getObrasPendentesDesenbolsoPorEstado($aEstado);
        } else {
            foreach($municipios as $municipio){
                $aMunicipio[] = $municipio['muncod'];
            }
            return getObrasPendentesDesenbolsoPorMunicipio($aMunicipio);
        }
    }

}


/**
 * Recupera obras com pendências de novos recursos
 * Obras com 60 dias sem atualização
 *
 * @param string $muncod
 * @return array - Obras pendentes
 */
function getObrasPendentesRecursosPorCpf($cpf){
    $arEmpid 	 = pegaEmpidPermitido($cpf);

    $empreendimento = new Empreendimento();
    $municipios = $empreendimento->pegaMuncodPorEmpid($arEmpid);

    $esfera = verificaEsferaEmpreendimento($arEmpid);

    $aMunicipio = array();
    $aEstado 	= array();
    if(is_array($municipios)){
        if( $esfera == 'E' ){
            foreach($municipios as $municipio){
                $aEstado[] = $municipio['estuf'];
            }
            return getObrasPendentesRecursosPorEstado($aEstado);
        } else {
            foreach($municipios as $municipio){
                $aMunicipio[] = $municipio['muncod'];
            }
            return getObrasPendentesRecursosPorMunicipio($aMunicipio);
        }
    }

}

/**
 * Recupera obras com pendências de novos recursos
 * Obras com 60 dias sem atualização
 *
 * @param string $muncod
 * @return array - Obras pendentes
 */
function getObrasPendentesRecursosPorMunicipio($aMunicipio)
{
    global $db;

    if($aMunicipio){
        $aMunicipio = (array) $aMunicipio;

        $sql = "
                    SELECT
                *
                FROM obras2.vm_total_pendencias
                WHERE
                empesfera = 'M'
                AND muncod in ('" . implode("', '", $aMunicipio) . "')
                AND pendencia = 'recursos'
                ";

        $result = $db->carregar($sql);
        $result = ($result) ? $result : array();
        return $result;
    }
    return array();
}

/**
 * Recupera obras com pendências de novos recursos
 * Obras com 60 dias sem atualização
 *
 * @param string $muncod
 * @return array - Obras pendentes
 */
function getObrasPendentesRecursosPorEstado($estuf){
    global $db;

    if($estuf){
        $estuf = (array) $estuf;

        $sql = "
                SELECT
                *
                FROM obras2.vm_total_pendencias
                WHERE
                empesfera = 'E'
                AND estuf in ('" . implode("', '", $estuf) . "')
                AND pendencia = 'recursos'
                ";

        $result = $db->carregar($sql);
        $result = ($result) ? $result : array();
        return $result;

    }
    return array();
}



/**
 * Recupera obras Inacabadas

 *
 * @param string $muncod
 * @return array - Obras Inacabadas
 */
function getObrasInacabadasPorCpf($cpf){


    $arEmpid 	 = pegaEmpidPermitido($cpf);

    $empreendimento = new Empreendimento();
    $municipios = $empreendimento->pegaMuncodPorEmpid($arEmpid);

    $esfera = verificaEsferaEmpreendimento($arEmpid);

    $aMunicipio = array();
    $aEstado 	= array();
    if(is_array($municipios)){
        if( $esfera == 'E' ){
            foreach($municipios as $municipio){
                $aEstado[] = $municipio['estuf'];
            }
            return getObrasInacabadasPorEstado($aEstado);
        } else {
            foreach($municipios as $municipio){
                $aMunicipio[] = $municipio['muncod'];
            }
            return getObrasInacabadasPorMunicipio($aMunicipio);
        }
    }

}


/**
 * Recupera obras Inacabadas

 * @param array $aMunicipio
 * @return array - Obras pendentes
 */
function getObrasInacabadasPorMunicipio($aMunicipio)
{

    global $db;

    if($aMunicipio){
        $aMunicipio = (array) $aMunicipio;

        $sql = "
                SELECT
                *
                FROM obras2.obras o
                join obras2.empreendimento emp on  o.empid = emp.empid
                join entidade.endereco ende on  ende.endid = emp.endid
                join territorios.municipio mun on  mun.muncod = ende.muncod
                join workflow.documento doc on doc.docid = o.docid
                join workflow.estadodocumento est on doc.esdid = est.esdid AND est.esdid = ".ESDID_OBJ_INACABADA."
                WHERE
                empesfera = 'M'
                AND o.obrstatus = 'A'
                AND o.obridpai IS NULL
                AND mun.muncod in ('" . implode("', '", $aMunicipio) . "')

                ";

        $result = $db->carregar($sql);
        $result = ($result) ? $result : array();
        return $result;
    }
    return array();
}


/**
 * Recupera obras Inacabadas

 * @param string $estuf
 * @return array - Obras pendentes
 */
function getObrasInacabadasPorEstado($estuf){
    global $db;

    if($estuf){
        $estuf = (array) $estuf;

        $sql = "
                SELECT
                *
                FROM obras2.obras o
                join obras2.empreendimento emp on  o.empid = emp.empid
                join entidade.endereco ende on  ende.endid = emp.endid
                join territorios.municipio mun on  mun.muncod = ende.muncod
                join workflow.documento doc on doc.docid = o.docid
                join workflow.estadodocumento est on doc.esdid = est.esdid AND est.esdid = ".ESDID_OBJ_INACABADA."
                WHERE
                emp.empesfera = 'E'
                AND ende.estuf in ('" . implode("', '", $estuf) . "')
               AND o.obrstatus = 'A'
                AND o.obridpai IS NULL

                ";

        $result = $db->carregar($sql);
        $result = ($result) ? $result : array();
        return $result;

    }
    return array();
}

/**
 * Recupera obras pendentes por CPF vinculado:
 *  Obras em vermelho ou
 *  Obras em Paralisadas ou
 *  Obras em planejamento pelo proponente a mais de 365 dias e que já receberam recursos
 *
 * @param string $cpf
 * @return array - Obras pendentes
 */
function getObrasPendentesPARPorCpf($cpf){

    $arEmpid 	 = pegaEmpidPermitido($cpf);

    $empreendimento = new Empreendimento();
    $municipios = $empreendimento->pegaMuncodPorEmpid($arEmpid);

    $esfera = verificaEsferaEmpreendimento($arEmpid);

    $aMunicipio = array();
    $aEstado 	= array();
    if(is_array($municipios)){
        if( $esfera == 'E' ){
            foreach($municipios as $municipio){
                $aEstado[] = $municipio['estuf'];
            }
            return getObrasPendentesPAR(null, $aEstado);
        } else {
            foreach($municipios as $municipio){
                $aMunicipio[] = $municipio['muncod'];
            }
            return getObrasPendentesPAR($aMunicipio);
        }
    }

}

/**
 * Recupera obras pendentes:
 *  Obras em vermelho ou
 *  Obras em Paralisadas ou
 *  Obras em planejamento pelo proponente a mais de 365 dias e que já receberam recursos
 *
 * @param string $muncod
 * @return array - Obras pendentes
 */
function getObrasPendentesPAR($aMunicipio, $aUF = null, $esfera = null){
    global $db;

    if(!$esfera){
        $esfera = !empty($aMunicipio) ? 'M' : 'E';
    }

    if($aMunicipio){
        $aMunicipio = (array) $aMunicipio;

        $where = "where o.muncod in ('" . implode("', '", $aMunicipio) . "')
                  and empesfera = '{$esfera}' ";
    } elseif($aUF){
        $aUF = (array) $aUF;

        $where = "where o.estuf in ('" . implode("', '", $aUF) . "')
                  and empesfera = '{$esfera}' ";
    } else {
        return array();
    }

    $sql = "select o.obrid, o.preid, o.obrnome, o.estuf, o.muncod, o.mundescricao, o.pendencia, coalesce(obr.obrpercentultvistoria, 0) as obrpercentultvistoria
            from obras2.vm_pendencia_obras o
            JOIN obras2.obras obr ON obr.obrid = o.obrid
            $where
            ";

    return $db->carregar($sql);
}


function verificaEsferaEmpreendimento($arEmpid){
    global $db;
    if( is_array($arEmpid) && !empty($arEmpid)){
        $sql = "SELECT DISTINCT empesfera FROM obras2.empreendimento WHERE empesfera IS NOT NULL AND empid in ( ".implode($arEmpid, ", ")." )";
        return $db->pegaUm($sql);
    }
}

function verificaObraEmenda($preid){
    global $db;

    $sql = "select count(*)
            from obras.preobra
            where ptoid in (44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,75,76)
            and preid = '$preid'";

    return $db->pegaUm($sql);
}

/**
 * Exibe mensagem de pendência de obras por município
 *
 * @param string $muncod
 */
function exibirAvisoPendencias($cpf, $mundod = '', $destinatario = 'S', $estuf = '', $esfera = '')
{
    global $db;

    if($mundod)
        $municipio = $db->pegaLinha("SELECT * FROM territorios.municipio WHERE muncod = '{$mundod}' ");

	if( $cpf != '' ){
	    $obrasPendentesPAR = getObrasPendentesPARPorCpf($cpf);
		$obrasPendentesDesenbolso = getObrasPendentesDesenbolsoPorCpf($cpf);
        $obrasPendentesPreenchimento = getObrasPendentesPreenchimentoPorCpf($cpf);
        $obrasPendentesRecursos = getObrasPendentesRecursosPorCpf($cpf);
        $obrasReformulacaoMI = getObrasReformulacaoPorCpf($cpf);
        $obrasSolicitacaoDiligencia = getObrasSolicitacaoDiligenciaPorCpf($cpf);
        $obrasInacabadas = getObrasInacabadasPorCpf($cpf);
	} elseif( $esfera == 'M' && $mundod != '' ){
		$descricao 	= "Município";
		$cargo 		= "Prefeito";
	    $obrasPendentesPAR = getObrasPendentesPAR($mundod);
		$obrasPendentesDesenbolso = getObrasPendentesDesenbolsoPorMunicipio($mundod);
        $obrasPendentesPreenchimento = getObrasPendentesPreenchimentoPorMunicipio($mundod);
        $obrasPendentesRecursos = getObrasPendentesRecursosPorMunicipio($mundod);
        $obrasReformulacaoMI = getObrasReformulacaoMunicipio($mundod);
        $obrasSolicitacaoDiligencia = getObrasSolicitacaoDiligenciaPorMunicipio($mundod);
        $obrasInacabadas = getObrasInacabadasPorMunicipio($mundod);
    } elseif( $esfera == 'E' && $estuf != '' ){
		$descricao 	= "Estado";
		$cargo 		= "Secretário";
	    $obrasPendentesPAR = getObrasPendentesPAR(null, $estuf);
		$obrasPendentesDesenbolso = getObrasPendentesDesenbolsoPorEstado($estuf);
        $obrasPendentesPreenchimento = getObrasPendentesPreenchimentoPorEstado($estuf);
        $obrasPendentesRecursos = getObrasPendentesRecursosPorEstado($estuf);
        $obrasInacabadas = getObrasInacabadasPorEstado($estuf);
	}
    $obrasPendentesPAR = (is_array($obrasPendentesPAR) && !empty($obrasPendentesPAR)) ? $obrasPendentesPAR : array();
    $obrasPendentesDesenbolso = (is_array($obrasPendentesDesenbolso) && !empty($obrasPendentesDesenbolso)) ? $obrasPendentesDesenbolso : array();
    $obrasPendentesPreenchimento = (is_array($obrasPendentesPreenchimento) && !empty($obrasPendentesPreenchimento)) ? $obrasPendentesPreenchimento : array();
    $obrasPendentesRecursos = (is_array($obrasPendentesRecursos) && !empty($obrasPendentesRecursos)) ? $obrasPendentesRecursos : array();
    $obrasInacabadas = (is_array($obrasInacabadas) && !empty($obrasInacabadas)) ? $obrasInacabadas : array();

    if($obrasPendentesPAR || $obrasPendentesDesenbolso || $obrasPendentesPreenchimento || $obrasPendentesRecursos || $obrasInacabadas){

        if($destinatario == 'S'){
            $texto = '<h4>Senhor '.$cargo.',</h4>

            <p style="margin: 10px 0px;">O seu '.$descricao.' já recebeu recursos para as obras listadas abaixo e as mesmas apresentam pendências em sua execução.</p>
            <p style="margin: 10px 0px;">Enquanto o problema não for sanado, o FNDE não procederá a análise de novas demandas de obras, tampouco efetuará novos Termos de Compromisso com seu '.$descricao.'.</p>
            <p style="margin: 10px 0px;">Caso a situação tenha sido resolvida, favor atualizar o módulo de obras que o sistema será imediatamente desbloqueado e sua obra será analisada.</p>';
        } elseif($destinatario == 'E'){
            $texto = '<h4>Senhor(a) Analista,</h4>
//
            <p style="margin: 10px 0px;">O '.$descricao.' em questão já recebeu recursos para as obras listadas abaixo e as mesmas apresentam pendências em sua execução.</p>
            <p style="margin: 10px 0px;">Enquanto o problema não for sanado, o FNDE não procederá a análise de novas demandas de obras, tampouco efetuará novos Termos de Compromisso com o '.$descricao.'.</p>
            <p style="margin: 10px 0px;">Caso a situação tenha sido resolvida, favor atualizar o módulo de obras que o sistema será imediatamente desbloqueado e sua obra será analisada.</p>';
        }

        ?>

        <style>

            .box.box-small {
                width: 20%;
            }

            .box.box-medium {
                width: 46%;
            }

            .box.box-large {
                width: 94.5%;
            }

            .box {
                FONT: 11pt Arial;
                -moz-border-radius: 20px;
                border-radius: 20px;
                padding: 10px;
                margin: 10px;
                float: left;
            }

            .box .box-header {
                text-align: center;
                color: #FFFFFF;
                height: 30px;
                font-weight: bold;
                font-size: 14px;
            }

            .box .box-header .box-header-options{
                cursor: pointer;
                float: right;
                margin: 0 8px 0 0;
            }

            .box .box-body {
                text-align: center;
                background-color: #FFFFFF;
                border-radius: 20px;
                border-radius: 5px;
                padding: 4px;
                text-align: center;
                min-height: 130px;
            }
            .box .box-body .box-body-title {
                font-weight: bold;
                font-size: 14px;
            }
            .box .box-body .box-body-subtitle {
                font-size: 11px;
            }

            .box.box-red {
                background-color: #EE3B3B;
            }

            .box.box-gray {
                background-color: #7C8BA2;
            }

            .box.box-black {
                background-color: #000000;
            }

            .box.box-yellow {
                background-color: #FFC200;
            }

            .box.box-green {
                background-color: #348300;
            }

            .box.box-purple {
                background-color: #6900AF;
            }

            .box.box-blue {
                background-color: #3871C8;
            }

            .box.box-orange {
                background-color: #FF8500;
            }
            .box p{
                margin: 0;
                padding: 0;
            }
            .print{
                background-color: #FFF;
                padding: 1px;
                border-bottom: 1px solid #000;
                border-right: 1px solid #000;
                border-top: 1px solid #CCC;
                border-left: 1px solid #CCC;
            }
        </style>

        <div id="dialog_obras" title="Obras com pendências">
            <h4>Prezado(a),</h4>
            <p style="margin: 10px 0px;">Os quadros abaixo apresentam as obras com problema no sistema.</p>
            <div class="centralizadora" style="float: left">
            <? if($obrasReformulacaoMI): ?>
                <div class="box box-blue box-small">
                    <div style="font-size: 12px" class="box-header">
                        Reformulação MI para Convencional
                    </div>
                    <div class="box-body">
                        <p class="box-body-title"><?=count($obrasReformulacaoMI)?> obras(s)</p>
                        <p>Seu município possui obras para reformulação MI para convencional. Para o preenchimento do pedido de solicitação da reformulação, acessar o MÓDULO - PAR no perfil do Prefeito Municipal.</p>
                    </div>
                    <div class="box-footer"></div>
                </div>
            <? endif; ?>
            <? if($obrasSolicitacaoDiligencia): ?>
                <div class="box box-gray box-small">
                    <div style="font-size: 12px" class="box-header">
                        Solicitações Diligênciadas
                    </div>
                    <div class="box-body">
                        <p class="box-body-title"><?= $obrasSolicitacaoDiligencia?> obra(s)</p>
                        <p>Obras com solicitações em diligência.</p>
                    </div>
                    <div class="box-footer"></div>
                </div>



            <? endif; ?>
            <? if($obrasInacabadas){ ?>
            <div style="background: #9C57D0" class="box box-small">
                <div style="font-size: 12px" class="box-header">
                    Obras inacabadas
                </div>
                <div class="box-body">
                    <p class="box-body-title"><?= count($obrasInacabadas)?> obra(s)</p>
                    <a href="#" style="font-size: 10px" title="Obras Inacabadas" rel="inacabadas_par" class="expand-prendencia">clique para ver</a>
                    <p>O município possui obras com status de INACABADA. Caso estas obras estejam concluídas, o município deve inserir vistoria com esta situação.</p>
                </div>
                <div class="box-footer"></div>
            </div>
            <? } ?>
            </div>
            <div style="clear: both"></div>

            <div class="box box-black box-small">
                <div class="box-header">
                    BLOQUEIO PAR
                </div>
                <div class="box-body">
                    <p class="box-body-title"><?=count($obrasPendentesPAR)?> obras(s)</p>
                    <p class="box-body-subtitle"><?if(count($obrasPendentesPAR)):?> <a href="#" title="Obras com pendências no PAR" rel="pendencia_par" class="expand-prendencia">clique para ver</a><?endif;?></p>
                    <p>Obras com problemas que impedem o FNDE de realizar análise de novas demandas e de efetivar novos Termos de Compromisso com sua entidade.</p>
                </div>
                <div class="box-footer"></div>
            </div>

            <div class="box box-red box-small">
                <div class="box-header">
                    BLOQUEIO RECURSOS
                </div>
                <div class="box-body">
                    <p class="box-body-title"><?=count($obrasPendentesRecursos)?> obras(s)</p>
                    <p class="box-body-subtitle"><?if(count($obrasPendentesRecursos)):?> <a href="#" title="Obras com pendências para novos recursos" rel="pendencia_recursos" class="expand-prendencia">clique para ver</a><?endif;?></p>
                    <p>Obras com problemas que impedem o FNDE de efetuar repasses dos recursos pactuados para QUAISQUER obras.</p>
                </div>
                <div class="box-footer"></div>
            </div>

            <div class="box box-orange box-small">
                <div class="box-header">
                    NOVOS DESEMBOLSOS
                </div>
                <div class="box-body">
                    <p class="box-body-title"><?=count($obrasPendentesDesenbolso)?> obras(s)</p>
                    <p class="box-body-subtitle"><?if(count($obrasPendentesDesenbolso)):?> <a href="#" title="Obras com pendências para novos recursos" rel="pendencia_desenbolso" class="expand-prendencia">clique para ver</a><?endif;?></p>
                    <p>Obras com problemas que impedem o FNDE de efetuar repasses dos recursos pactuados para estas obras</p>
                </div>
                <div class="box-footer"></div>
            </div>

            <div class="box box-yellow box-small">
                <div class="box-header">
                    PREENCHIMENTO
                </div>
                <div class="box-body">
                    <p class="box-body-title"><?=count($obrasPendentesPreenchimento)?> obras(s)</p>
                    <p class="box-body-subtitle"><?if(count($obrasPendentesPreenchimento)):?> <a href="#" title="Obras com pendências no preenchimento" rel="pendencia_preenchimento" class="expand-prendencia">clique para ver</a><?endif;?></p>
                    <p>Obras com problemas nas informações prestadas no sistema.</p>
                </div>
                <div class="box-footer"></div>
            </div>

            <div style="clear: both"></div>

            <? if ($obrasPendentesPAR): ?>
                <div id="pendencia_par" class="lista-obras" style="display: none">
                    <div class="box box-red box-large">
                        <div class="box-header">
                            BLOQUEIO PAR
                            <span class="box-header-options print"><img title="imprimir" rel="Obras com pendências no PAR" src="/imagens/print.png" /></span>
                        </div>
                        <div class="box-body">
                            <table class="table table-striped table-bordered table-hover">
                                <thead>
                                <tr>
                                    <th>Obra</th>
                                    <th>Descrição</th>
                                    <th>UF</th>
                                    <th>Município</th>
                                    <th>Exec.(%)</th>
                                    <th>Situação</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($obrasPendentesPAR as $dados) { ?>
                                    <tr>
                                        <td>
                                            <a target="_blank" href="/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=<?php echo $dados['obrid']; ?>"><?php echo $dados['obrid'] ?></a>
                                        </td>
                                        <td><?php echo $dados['obrnome']; ?></td>
                                        <td><?php echo $dados['estuf']; ?></td>
                                        <td><?php echo $dados['mundescricao']; ?></td>
                                        <td align="right"><?php echo simec_number_format($dados['obrpercentultvistoria'], 2, ',', '.'); ?></td>
                                        <td><span style="color: red;"><?php echo $dados['pendencia']; ?></span></td>

                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                        <div class="box-footer"></div>
                    </div>
                </div>
                <div style="clear: both"></div>
            <? endif; ?>


            <? if ($obrasInacabadas): ?>
                <div id="inacabadas_par" class="lista-obras" style="display: none">
                    <div class="box box-red box-large">
                        <div class="box-header">
                            OBRAS INACABADAS
                            <span class="box-header-options print"><img title="imprimir" rel="Obras com pendências no PAR" src="/imagens/print.png" /></span>
                        </div>
                        <div class="box-body">
                            <table class="table table-striped table-bordered table-hover">
                                <thead>
                                <tr>
                                    <th>Obra</th>
                                    <th>Descrição</th>
                                    <th>UF</th>
                                    <th>Município</th>

                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($obrasInacabadas as $dados) { ?>
                                    <tr>
                                        <td>
                                            <a target="_blank" href="/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=<?php echo $dados['obrid']; ?>"><?php echo $dados['obrid'] ?></a>
                                        </td>
                                        <td><?php echo $dados['obrnome']; ?></td>
                                        <td><?php echo $dados['estuf']; ?></td>
                                        <td><?php echo $dados['mundescricao']; ?></td>

                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                        <div class="box-footer"></div>
                    </div>
                </div>
                <div style="clear: both"></div>
            <? endif; ?>

            <? if($obrasPendentesDesenbolso): ?>
            <div id="pendencia_desenbolso" class="lista-obras" style="display: none">
                <div class="box box-orange box-large">
                    <div class="box-header">
                        NOVOS DESEMBOLSOS
                        <span class="box-header-options print"><img title="imprimir" rel="Obras com pendências para novos desenvolsos" src="/imagens/print.png" /></span>
                    </div>
                    <div class="box-body">
                        <table class="table table-striped table-bordered table-hover" style="width: 723px;">
                            <thead>
                            <tr>
                                <th>Obra</th>
                                <th>Descrição</th>
                                <th>UF</th>
                                <th>Município</th>
                                <th>Exec.(%)</th>
                                <th>Situação</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($obrasPendentesDesenbolso as $dados) { ?>
                                <tr>
                                    <td>
                                        <a target="_blank" href="/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=<?php echo $dados['obrid']; ?>"><?php echo $dados['obrid'] ?></a>
                                    </td>
                                    <td><?php echo $dados['predescricao']; ?></td>
                                    <td><?php echo $dados['estuf']; ?></td>
                                    <td><?php echo $dados['descricao']; ?></td>
                                    <td align="right"><?php echo simec_number_format($dados['obrpercentultvistoria'], 2, ',', '.'); ?></td>
                                    <td><span style="color: red;"><?php echo $dados['situacao']; ?></span></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
            <div style="clear: both"></div>
            <? endif; ?>

            <? if($obrasPendentesRecursos): ?>
                <div id="pendencia_recursos" class="lista-obras" style="display: none">
                    <div class="box box-yellow box-large">
                        <div class="box-header">
                            PREENCHIMENTO
                            <span class="box-header-options print"><img title="imprimir" rel="Obras com pendências de recursos" src="/imagens/print.png" /></span>
                        </div>
                        <div class="box-body">
                            <table class="table table-striped table-bordered table-hover" style="width: 723px;">
                                <thead>
                                <tr>
                                    <th>Obra</th>
                                    <th>Descrição</th>
                                    <th>UF</th>
                                    <th>Município</th>
                                    <th>Exec.(%)</th>
                                    <th>Situação</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($obrasPendentesRecursos as $dados) { ?>
                                    <tr>
                                        <td>
                                            <a target="_blank" href="/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=<?php echo $dados['obrid']; ?>"><?php echo $dados['obrid'] ?></a>
                                        </td>
                                        <td><?php echo $dados['predescricao']; ?></td>
                                        <td><?php echo $dados['estuf']; ?></td>
                                        <td><?php echo $dados['descricao']; ?></td>
                                        <td align="right"><?php echo simec_number_format($dados['obrpercentultvistoria'], 2, ',', '.'); ?></td>
                                        <td><span style="color: red;"><?php echo $dados['situacao']; ?></span></td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>


                <div style="clear: both"></div>
            <? endif; ?>

            <? if($obrasPendentesPreenchimento): ?>
            <div id="pendencia_preenchimento" class="lista-obras" style="display: none">
                <div class="box box-yellow box-large">
                    <div class="box-header">
                        PREENCHIMENTO
                        <span class="box-header-options print"><img title="imprimir" rel="Obras com pendências no preenchimento" src="/imagens/print.png" /></span>
                    </div>
                    <div class="box-body">
                        <table class="table table-striped table-bordered table-hover" style="width: 723px;">
                            <thead>
                            <tr>
                                <th>Obra</th>
                                <th>Descrição</th>
                                <th>UF</th>
                                <th>Município</th>
                                <th>Exec.(%)</th>
                                <th>Situação</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($obrasPendentesPreenchimento as $dados) { ?>
                                <tr>
                                    <td>
                                        <a target="_blank" href="/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=<?php echo $dados['obrid']; ?>"><?php echo $dados['obrid'] ?></a>
                                    </td>
                                    <td><?php echo $dados['predescricao']; ?></td>
                                    <td><?php echo $dados['estuf']; ?></td>
                                    <td><?php echo $dados['descricao']; ?></td>
                                    <td align="right"><?php echo simec_number_format($dados['obrpercentultvistoria'], 2, ',', '.'); ?></td>
                                    <td><span style="color: red;"><?php echo $dados['situacao']; ?></span></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>


            <div style="clear: both"></div>
            <? endif; ?>

            <div style="clear: both"></div>

            <div id="print-popup" style="display: none">
                <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
                <link rel="stylesheet" type="text/css" media="screen, print" href="../includes/Estilo.css">
                   <?php echo monta_cabecalho_relatorio(100); ?>
                <br />
                <table class="tabela" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%">
                    <tr>
                        <td colspan="2" width="100%" align="center"><label class="TituloTela" id="TituloTela" style="color:#000000;"></label></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" >Tipo de ensino:</td>
                        <td class="print-ensino" style="width: 100%;">Educação Básica</td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" >Município - UF:</td>
                        <td class="print-uf" style="width: 100%;"><?= ($municipio) ? $municipio['mundescricao'] . ' - ' . $municipio['estuf'] : ''; ?><?= $estuf ?></td>
                    </tr>
                    <tr>
                        <td id="print-content" colspan="2">

                        </td>
                    </tr>
                </table>
            </div>

            <p>Atenciosamente.<br />
            Equipe PAR MEC/FNDE</p>
        </div>

        <link href="/library/jquery/jquery-ui-1.10.3/themes/custom-theme/jquery-ui-1.10.3.custom.min.css" rel="stylesheet">
        <script language="javascript" type="text/javascript" src="/includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>


        <script type="text/javascript">

            function Popup(data, title)
            {
                if(window.windowPendencia) window.windowPendencia.close();
                window.windowPendencia = window.open('', 'Obras com Pendências', 'height=648,width=1024,scrollbars=yes');
                $('#print-content').empty().html(data);
                $('#print-popup .TituloTela').html(title);
                window.windowPendencia.document.write($('#print-popup').html());
                return true;
            }

            jQuery(function(){

                jQuery(".expand-prendencia").click(function(e){
                    e.preventDefault();
                    jQuery('#' + $(this).attr('rel')).find('.box-body').find('table').css('width', '100%');
                    Popup(jQuery('#' + $(this).attr('rel')).find('.box-body').html(), $(this).attr('title'));
                })

                var count;

                if(jQuery(".centralizadora .box").length == 3)
                {
                    jQuery(".centralizadora").css({ "margin-left": "60px" });
                    jQuery(".centralizadora .box").css({ "width": "200px" });
                }
                if(jQuery(".centralizadora .box").length == 2)
                {
                    jQuery(".centralizadora").css({ "margin-left": "180px" });
                    jQuery(".centralizadora .box").css({ "width": "200px" });
                }
                if(jQuery(".centralizadora .box").length == 1)
                {
                    jQuery(".centralizadora").css({ "margin-left": "300px" });
                    jQuery(".centralizadora .box").css({ "width": "200px" });
                }




                jQuery("#dialog_obras").dialog({
                    modal: true,
                    width: 880,
                    buttons: {
                        OK: function() {
                            jQuery(this).dialog( "close" );

                        }
                    }
                });

                $1_11(".ui-button-text").click(function(){
                    $1_11(this).closest('.ui-dialog').find(".ui-icon-closethick").trigger('click');
                });


            });

        </script>

    <?php } // endif($obrasPendentes)
}

function getSaldoProcesso($processo)
{
    global $db;

    //$processo = preg_replace( "[^0-9]", "", $processo);
    $processo = str_replace(".","", $processo);
    $processo = str_replace("/","", $processo);
    $processo = str_replace("-","", $processo);
    if($processo){
        $sql = "select po.ppaprocesso, po.ppacnpj, po.pparazaosocial, po.ppabanco, po.ppaagencia, po.ppaconta,
                    po.ppasaldoconta, po.ppasaldofundos, po.ppasaldopoupanca, po.ppasaldordbcdb
                from obras2.pagamentopac po
                where
                    po.ppaprocesso = '{$processo}'";

        return $db->pegaLinha($sql);
    }
    return null;
}

function getExtratoProcesso($processo, $order = 'dfidatasaldo desc')
{
    global $db;

    //$processo = preg_replace( "[^0-9]", "", $processo);
    $processo = str_replace(".","", $processo);
    $processo = str_replace("/","", $processo);
    $processo = str_replace("-","", $processo);
    if($processo){
        $sql = "select * from(
        		select dfi.*, (dfi.dfisaldoconta + dfi.dfisaldofundo + dfi.dfisaldopoupanca + dfi.dfisaldordbcdb) AS saldo, iue.iuenome as razao_social
                from painel.dadosfinanceirosconvenios dfi
                    left join par.instrumentounidadeentidade iue on iue.iuecnpj = dfi.dficnpj
                where dfi.dfiprocesso = '{$processo}'
                --order by {$order}
        		) as foo
        		order by {$order}, saldo desc";
		
        return $db->carregar($sql);
    }
    return array();
}

function getDadosObrasPorProcesso($processo)
{
    global $db;

    //$processo = preg_replace( "[^0-9]", "", $processo);
    $processo = str_replace(".","", $processo);
	$processo = str_replace("/","", $processo);
	$processo = str_replace("-","", $processo);

//        -- 23400001804201261 - SUBAÇÃO
//        -- 23400006087201344 - PAC
//        -- 23400004307201214 - PAR
//    $processo = '23400001804201261';
    if($processo){
        $sql = "
                -- OBRAS PAC DO PROCESSO
                select distinct 'Obras PAC do Processo' as tipo, pre.preid, pre.obrid, pre.estuf, pre.muncod, m.mundescricao, ed.esddsc,
                       case
                           when coalesce(o.obrnome, '') != '' then o.obrnome
                           else pre.predescricao
                       end as descricao
                from par.processoobraspaccomposicao pop
                    inner join par.processoobra po on po.proid = pop.proid and po.prostatus = 'A'
                    inner join obras.preobra pre on pre.preid = pop.preid
                    inner join territorios.municipio m on m.muncod = pre.muncod
                    left  join obras2.obras o on o.obrid = pre.obrid
                    left  join workflow.documento d on d.docid = o.docid
                    left  join workflow.estadodocumento ed on ed.esdid = d.esdid
                where 
                pop.pocstatus = 'A' and
                po.pronumeroprocesso = '{$processo}'

                union

                -- OBRAS PAR DO PROCESSO
                select distinct 'Obras PAR do Processo' as tipo, pre.preid, pre.obrid, pre.estuf, pre.muncod, m.mundescricao, ed.esddsc,
                      case
                          when coalesce(o.obrnome, '') != '' then o.obrnome
                          else pre.predescricao
                      end as descricao
                from par.processoobrasparcomposicao pop
                        inner join par.processoobraspar po on po.proid = pop.proid and po.prostatus = 'A'
                        inner join obras.preobra pre on pre.preid = pop.preid
                        inner join territorios.municipio m on m.muncod = pre.muncod
                        left  join obras2.obras o on o.obrid = pre.obrid
                        left  join workflow.documento d on d.docid = o.docid
                        left  join workflow.estadodocumento ed on ed.esdid = d.esdid
                where pop.pocstatus = 'A' and po.pronumeroprocesso = '{$processo}'

                union

                -- SUBAÇÕES DO PROCESSO
                select distinct 'Subações do Processo' as tipo, sba.sbaid, sbd.sbdid, m.estuf, m.muncod, m.mundescricao, ed.esddsc,sba.sbadsc as descricao
                from par.processoparcomposicao pop
                        inner join par.processopar po on po.prpid = pop.prpid and po.prpstatus = 'A'
                        inner join par.subacaodetalhe sbd on sbd.sbdid = pop.sbdid
                        inner join par.subacao        sba on sba.sbaid = sbd.sbaid
                        inner join territorios.municipio m on m.muncod = po.muncod
                        left  join workflow.documento d on d.docid = sba.docid
                        left  join workflow.estadodocumento ed on ed.esdid = d.esdid
                where pop.ppcstatus = 'A' and po.prpnumeroprocesso = '$processo'

                ";

        return $db->carregar($sql);
    }
    return array();
}

function exibirDadosFuncionalProgramatica($funcional, $ptres){
	global $db;

	$arrFuncional = explode('.', $funcional);
	$esfcod = $arrFuncional[0]; 
	$unicod = $arrFuncional[1];
	$funcod = $arrFuncional[2];
	$sfucod = $arrFuncional[3];
	$prgcod = $arrFuncional[4];
	$acacod = $arrFuncional[5];
	$loccod = $arrFuncional[6];
	
	$sql = "SELECT distinct
			    a.esfcod || '.' || a.unicod || '.' || a.funcod || '.' || a.sfucod || '.' || a.prgcod || '.' || a.acacod || '.' || a.loccod as funcional,
			    a.esfcod || ' - ' || es.esfdsc as esfera,
			    a.unicod || ' - ' || un.unidsc as unidade,
			    a.funcod || ' - ' || pf.fundsc as funcao,
			    a.sfucod || ' - ' || psf.sfudsc as subfuncao,
			    a.prgcod || ' - ' || pg.prgdsc as programa,
			    a.acacod || ' - ' || a.acadsc as acao,
			    a.loccod || ' - ' || lo.locdsc as localizador
			FROM monitora.acao a
			    inner join public.esfera es on es.esfcod = a.esfcod and es.esfstatus = 'A'
			    inner join public.unidade un on un.unicod = a.unicod and un.unistatus = 'A'
			    inner join public.ppafuncao pf on pf.funcod = a.funcod
			    inner join public.ppasubfuncao psf on psf.sfucod = a.sfucod
			    inner join monitora.programa pg on pg.prgcod = a.prgcod and pg.prgstatus = 'A'
			    left join public.localizador lo on lo.loccod = a.loccod
			WHERE
				a.esfcod = $esfcod
			    and a.unicod = '$unicod'
			    and a.funcod = '$funcod'
			    and a.sfucod = '$sfucod'
			    and a.prgcod = '$prgcod'
			    and a.acacod = '$acacod'
			    and a.loccod = '$loccod'";
	
	$arFuncional = $db->pegaLinha($sql);
	?>
	<table class="table table-bordered">
		<tr>
			<td class="subtitulodireita" width="15%">Esfera:</td>
			<td width="35%"><?php echo $arFuncional['esfera']; ?></td>
			<td class="subtitulodireita" width="15%">Unidade:</td>
			<td width="35%"><?php echo $arFuncional['unidade']; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Função:</td>
			<td><?php echo $arFuncional['funcao']; ?></td>
			<td class="subtitulodireita">subFunção:</td>
			<td><?php echo $arFuncional['subfuncao']; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Programa:</td>
			<td><?php echo $arFuncional['programa']; ?></td>
			<td class="subtitulodireita">Localizador:</td>
			<td><?php echo $arFuncional['localizador']; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Ação:</td>
			<td colspan="3"><?php echo $arFuncional['acao']; ?></td>
		</tr>
	</table>
	<?php
	
	$sql = "SELECT distinct
			    pt.ptres, 
			    pt.ptrano,
			    po.plocodigo ||' - '|| po.plotitulo as orcamentario,
			    pi.plicod ||' - '|| pi.plititulo as plano
			FROM monitora.acao a
			    inner join monitora.ptres pt on pt.acaid = a.acaid and pt.ptrstatus = 'A'
			    left join monitora.planoorcamentario po ON po.acaid = pt.acaid AND po.plocodigo = pt.plocod and po.plostatus = 'A'
			    inner join monitora.pi_planointernoptres pip ON pt.ptrid = pip.ptrid
			    inner join monitora.pi_planointerno pi ON pip.pliid = pi.pliid and pi.plistatus = 'A'
			WHERE
				pt.ptres = '$ptres'";
	$arrPlano = $db->carregar($sql);
	$arrPlano = $arrPlano ? $arrPlano : array();
	?>
	<div id="accordion">
	  <h3>Dados Orçamentário</h3>
	  <div id="identificacao" class="collapse panel-collapse" style="overflow-x: auto;">
	<table class="table table-bordered table-hover table-striped">
		<tr>
			<td>PTRES</td>
			<td>Ano PTRES</td>
			<td>Plano Orçamentário</td>
			<td>Plano Interno</td>
		</tr>
<?php	foreach($arrPlano as $dado) { ?>
			<tr>
				<td><?php echo $dado['ptres']; ?></td>
				<td><?php echo $dado['ptrano']; ?></td>
				<td><?php echo $dado['orcamentario']; ?></td>
				<td><?php echo $dado['plano']; ?></td>
			</tr>
		<?php } ?>
	</table>
	<table class="table table-bordered table-hover table-striped">
		<tr>
			<td style="text-align: right;" width="90%"><b>Total de Registros:</b></td>
			<td style="text-align: center;"><?php echo sizeof($arrPlano); ?></td>
		</tr>
	</table>
	  </div>
	</div>
	<script>
	  jQuery(function() {
		  jQuery( "#accordion" ).accordion({
		    	collapsible: true,
				clearStyle: true,
				active: 1
		    });
		  jQuery('.panel-collapse').collapse('show');
	  });
  </script>
	<?php 
}

function exibirSaldoProcesso($processo, $incluirHighcharts = true)
{
    $dados = getExtratoProcesso($processo);
    $dadosObras = getDadosObrasPorProcesso($processo);

    echo '<div class="table-responsive" style="max-height: 500px;">';

            if (is_array($dados)) {
                $oData = new Data();
                $oGrafico = new Grafico(Grafico::K_TIPO_AREA, $incluirHighcharts);

                echo '<h4>Saldos do Processo</h4>

                <div style="width: 900px;">';
                    $oGrafico->setAgrupadores(array('categoria' => 'dfimesanosaldo', 'name' => 'dfiprocesso', 'valor' => 'saldo'))
                             ->setFormatoTooltip("function() { return '<span>' + this.x + '</b><br /><span style=\"color: ' + this.series.color + '\">Valor</span>: <b>' + number_format(this.y, 2, ',', '.') + '</b>'; }")
                             ->gerarGrafico(getExtratoProcesso($processo, 'dfidatasaldo'));
                echo '</div>';

                ?>
                    <table class="table table-bordered table-hover table-striped">
                        <tr>
                            <td nowrap="nowrap"><img class="img_detalhe" src="/imagens/mais.gif" style="margin-right: 5px;" />Processo</td>
                            <td>CNPJ</td>
                            <td>Razão Social</td>
                            <td>Banco</td>
                            <td>Agência</td>
                            <td>Conta</td>
                            <td>Data</td>
                            <td>Saldo da Conta</td>
                            <td>Saldo Fundos</td>
                            <td>Saldo da Poupança</td>
                            <td>Saldo CDB</td>
                            <td style="font-weight: bold">Saldo TOTAL</td>
                        </tr>
                        <?php
                        foreach($dados as $count => $dado) {
                            $saldo = $dado['dfisaldoconta'] + $dado['dfisaldofundo'] + $dado['dfisaldopoupanca'] + $dado['dfisaldordbcdb'];
                            ?>
                            <tr <?php echo $count ? 'class="detalhe_saldo"' : ''; ?>>
                                <td><?php echo $dado['dfiprocesso']; ?></td>
                                <td><?php echo $dado['dficnpj']; ?></td>
                                <td><?php echo $dado['razao_social']; ?></td>
                                <td><?php echo $dado['dfibanco']; ?></td>
                                <td><?php echo $dado['dfiagencia']; ?></td>
                                <td><?php echo $dado['dficonta']; ?></td>
                                <td><?php echo $oData->formataData($dado['dfidatasaldo']); ?></td>
                                <td align="right"><?php echo simec_number_format($dado['dfisaldoconta'], 2, ',', '.'); ?></td>
                                <td align="right"><?php echo simec_number_format($dado['dfisaldofundo'], 2, ',', '.'); ?></td>
                                <td align="right"><?php echo simec_number_format($dado['dfisaldopoupanca'], 2, ',', '.'); ?></td>
                                <td align="right"><?php echo simec_number_format($dado['dfisaldordbcdb'], 2, ',', '.'); ?></td>
                                <td align="right"  style="font-weight: bold"><?php echo simec_number_format($saldo, 2, ',', '.'); ?></td>
                            </tr>
                        <?php } ?>
                    </table>
            <?php } else {
                echo '<h4 style="color: red;">Processo sem saldo</h4>';
            }
        echo '
        <div>

        </div>
    <div class="colu-md-9">';
        if ($dadosObras) {
            $current = current($dadosObras);
            ?>
            <h4><?php echo $current['tipo']; ?></h4>
            <div class="table-responsive">
                <table class="table table-bordered table-hover table-striped">
                    <tr>
                        <th>PREID/SBAID</th>
                        <th>OBRID/SBDID</th>
                        <th>Descrição</th>
                        <th>Município</th>
                        <th>Situação</th>
                    </tr>
                    <?php foreach($dadosObras as $obra) { ?>
                        <tr>
                            <td><?php echo $obra['preid']; ?></td>
                            <td><?php echo $obra['obrid']; ?></td>
                            <td><?php echo $obra['descricao']; ?></td>
                            <td><?php echo $obra['estuf'] . ' - ' . $obra['mundescricao']; ?></td>
                            <td><?php echo $obra['esddsc']; ?></td>
                        </tr>
                    <?php } ?>
                </table>
            </div>
        <?php } else {
            echo '<p>Nenhum registro encontrado</p>';
        }
    echo '</div>
    </div>'; ?>

    <script type="text/javascript">
        jQuery(function(){
            jQuery('.detalhe_saldo').hide();
            jQuery('.img_detalhe').click(function(){
                if(jQuery(this).attr('src') == '/imagens/mais.gif'){
                    jQuery('.detalhe_saldo').show();
                    jQuery(this).attr('src', '/imagens/menos.gif');
                } else {
                    jQuery('.detalhe_saldo').hide();
                    jQuery(this).attr('src', '/imagens/mais.gif');
                }
            });
        });
    </script>

<?php }

function prepararDetalheProcesso(){

?>
    <div id="dialog_detalhe_processo"></div>
    <style>
    	.processo_detalhe{
     		cursor:pointer;
     		color:blue;
    	}
    	.processo_detalhe:hover{
     		cursor:pointer;
     		color:#87CEFA;
    	}
    </style>

            <script language="javascript" src="/includes/Highcharts-3.0.0/js/highcharts.js"></script>
            <script language="javascript" src="/includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
            <script language="javascript" src="/estrutura/js/funcoes.js"></script>

        
    <script type="text/javascript">
        if( jQuery.fn.jquery == "1.11.1"){
            var inc = '<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.min.js" /><link href="/library/jquery/jquery-ui-1.10.3/themes/custom-theme/jquery-ui-1.10.3.custom.css" rel="stylesheet" />';
        } else {
            var inc = '<script language="javascript" type="text/javascript" src="/includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js" /><link href="/library/jquery/jquery-ui-1.10.3/themes/custom-theme/jquery-ui-1.10.3.custom.min.css" rel="stylesheet" />';
        }
        jQuery('head').append(inc);
        jQuery(function(){
              if( jQuery.fn.jquery == "1.11.1" || jQuery.fn.jquery == "1.10.2" ){
                jQuery('body').on('click','.processo_detalhe', function(){
                    var nrprocesso = jQuery(this).html();
                    jQuery("#dialog_detalhe_processo").load('/obras2/ajax.php?buscasaldoprocesso=' + nrprocesso, function(){
                        jQuery("#dialog_detalhe_processo").dialog({
                            modal: true,
                            width: 1000,
                            title: 'Detalhes do Processo ' + nrprocesso,
                            buttons: {
                                Fechar: function() {
                                    jQuery("#dialog_detalhe_processo").html('');
                                    jQuery( this ).dialog( "close" );
                                }
                            }
                        });
                    });
                });  
              }else{
                jQuery(".processo_detalhe").live('click', function(){
                    var nrprocesso = jQuery(this).html();
                    jQuery("#dialog_detalhe_processo").load('/obras2/ajax.php?buscasaldoprocesso=' + nrprocesso, function(){
                        jQuery("#dialog_detalhe_processo").dialog({
                            modal: true,
                            width: 1000,
                            title: 'Detalhes do Processo ' + nrprocesso,
                            buttons: {
                                Fechar: function() {
                                    jQuery("#dialog_detalhe_processo").html('');
                                    jQuery( this ).dialog( "close" );
                                }
                            }
                        });
                    });
                });    
              }

        });
    </script>
<?php
}

function prepararDetalhePendenciasObras(){

?>
    <div id="div_detalhe_pendencias_obras"></div>
    <script type="text/javascript">
        jQuery(function(){
            if( jQuery.fn.jquery == "1.11.1" || jQuery.fn.jquery == "1.10.2" ){
                jQuery('body').on('click',".detalhar_pendencias_obras", function(){
                    jQuery("#div_detalhe_pendencias_obras").load('/obras2/ajax.php?detalhar_pendencias_obras=1&muncod='+jQuery(this).attr('muncod')+'&estuf='+jQuery(this).attr('estuf'));
                });
            }else{
                    jQuery(".detalhar_pendencias_obras").live('click', function(){
                    jQuery("#div_detalhe_pendencias_obras").load('/obras2/ajax.php?detalhar_pendencias_obras=1&muncod='+jQuery(this).attr('muncod')+'&estuf='+jQuery(this).attr('estuf'));
                });
            }
        });
    </script>
<?php
}

function prepararDetalheFuncionalProgramatica(){

	?>
    <div id="dialog_detalhe_funcionalprogramatica"></div>
    <style>
    	.funcionalprogramatica_detalhe{
     		cursor:pointer;
     		color:blue;
    	}
    	.funcionalprogramatica_detalhe:hover{
     		cursor:pointer;
     		color:#87CEFA;
    	}
    </style>
    
    <script language="javascript" src="/estrutura/js/funcoes.js"></script>
        
    <script type="text/javascript">
        if( jQuery.fn.jquery == "1.11.1" ){
            var inc = '<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.min.js" /><link href="/library/jquery/jquery-ui-1.10.3/themes/custom-theme/jquery-ui-1.10.3.custom.css" rel="stylesheet" />';
        } else {
            var inc = '<script language="javascript" type="text/javascript" src="/includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js" /><link href="/library/jquery/jquery-ui-1.10.3/themes/custom-theme/jquery-ui-1.10.3.custom.min.css" rel="stylesheet" />';
        }
        jQuery('head').append(inc);
        jQuery(function(){
              if( jQuery.fn.jquery == "1.11.1" || jQuery.fn.jquery == "1.10.2" ){
                jQuery('body').on('click','.funcionalprogramatica_detalhe', function(){
                    var nrfuncionalprogramatica = jQuery(this).html();
                    var ptres = jQuery(this).attr('id');
                    
                    jQuery("#dialog_detalhe_funcionalprogramatica").load('/obras2/ajax.php?buscasaldofuncionalprogramatica=' + nrfuncionalprogramatica+'&ptres='+ptres, function(){
                        jQuery("#dialog_detalhe_funcionalprogramatica").dialog({
                            modal: true,
                            width: 1000,
                            title: 'Detalhes do Funcional Programática ' + nrfuncionalprogramatica,
                            buttons: {
                                Fechar: function() {
                                    jQuery("#dialog_detalhe_funcionalprogramatica").html('');
                                    jQuery( this ).dialog( "close" );
                                }
                            }
                        });
                    });
                });  
              }else{
                jQuery(".funcionalprogramatica_detalhe").live('click', function(){
                    var nrfuncionalprogramatica = jQuery(this).html();
                    var ptres = jQuery(this).attr('id');
                    
                    jQuery("#dialog_detalhe_funcionalprogramatica").load('/obras2/ajax.php?buscasaldofuncionalprogramatica=' + nrfuncionalprogramatica+'&ptres='+ptres, function(){
                        jQuery("#dialog_detalhe_funcionalprogramatica").dialog({
                            modal: true,
                            width: 1000,
                            title: 'Detalhes do Funcional Programática ' + nrfuncionalprogramatica,
                            buttons: {
                                Fechar: function() {
                                    jQuery("#dialog_detalhe_funcionalprogramatica").html('');
                                    jQuery( this ).dialog( "close" );
                                }
                            }
                        });
                    });
                });    
              }

        });
    </script>
<?php
}

function montarAvisoCabecalho($esfera, $muncod, $estuf = null, $inuid = null, $proid = null, $tipo = null, $style = null)
{
	
    global $db;
    if($inuid){
        $dadoEntidade = $db->pegaLinha("SELECT iu.itrid, iu.muncod, iu.estuf FROM par.instrumentounidade iu WHERE iu.inuid = ".$inuid);
        $esfera = $dadoEntidade['itrid'] == 2 ? 'M' : 'E';
        $muncod = $dadoEntidade['muncod'];
        $estuf  = $dadoEntidade['estuf'];
    } elseif($proid){
        if($tipo == 'obras'){
            $dadoEntidade = $db->pegaLinha("SELECT iu.itrid, iu.muncod, iu.estuf FROM par.instrumentounidade iu INNER JOIN par.processoobraspar pro ON pro.inuid = iu.inuid WHERE pro.prostatus = 'A' and pro.proid = ".$proid);
        } else {
            $dadoEntidade = $db->pegaLinha("SELECT muncod, estuf FROM par.processoobra pro WHERE prostatus = 'A' and pro.proid = ".$proid);
        }

        $esfera = $dadoEntidade['muncod'] ? 'M' : 'E';
        $muncod = $dadoEntidade['muncod'];
        $estuf  = $dadoEntidade['estuf'];
    }


    $obrasPendentes = '';
    if($esfera == 'M' && $muncod){
        $descricao = 'município';
        $link = 'muncod="' . $muncod . '"';
        $obrasPendentes = getObrasPendentesPAR($muncod);
    } elseif($esfera == 'E' && $estuf) {
        $descricao = 'estado';
        $link = 'estuf="' . $estuf . '"';
        $obrasPendentes = getObrasPendentesPAR(null, $estuf);
    }

    if($obrasPendentes){

		if($style == 'acompanhamento')
		{
			return '
                      <b style=\'margin-left:40px\'>Pendência de Obras:</b>	
                        			<a class="detalhar_pendencias_obras" href="#" style="color: #red; text-decoration: underline;" '
                        		 . $link . '> <img src=../imagens/workflow/2.png title="Lista de Obras com pendência"  style=cursor:pointer;></a> ';	
		}
		else 
		{
		
	        echo '<div style="background: #f00; color: #fff; margin: 0 10px; padding: 10px; text-align: justify;">
	                    O seu ' . $descricao . ' já recebeu recursos para as obras <a class="detalhar_pendencias_obras" href="#" style="color: #fff; text-decoration: underline;" ' . $link . '>AQUI LISTADAS</a> e as mesmas apresentam pendências em sua execução.
	                    Enquanto o problema não for sanado, o FNDE não procederá a análise de novas demandas de obras, tampouco efetuará novos Termos de Compromisso com seu Estado/Município.
	                    Caso a situação tenha sido resolvida, favor atualizar o módulo de obras que o sistema será imediatamente desbloqueado e sua obra será analisada.
	                    -  Equipe PAR MEC/FNDE.
	                </div>';
				
		}
    }
}

function montarPainel($uf)
{

//    echo '<iframe src="http://www.cidades.ibge.gov.br/xtras/perfil.php?lang=&codmun=120001&search=acre|acrelandia" width="800" height="600"></iframe>';

}

function pegaEmpidPermitido($cpf)
{
    global $db;
    if (possuiPerfilGestorUnidade($cpf)) {
        $sql = "SELECT e.empid FROM obras2.obras o
            JOIN obras2.empreendimento e ON e.empid = o.empid
            JOIN obras2.usuarioresponsabilidade urs ON urs.rpustatus = 'A' AND
                                    urs.usucpf = '$cpf' AND
                                    urs.pflcod IN (" . PFLCOD_GESTOR_UNIDADE . ") AND
                                    (urs.entid = e.entidunidade )
            AND o.obrstatus = 'A' AND o.obridpai IS NULL";
        $arEmpid = $db->carregarColuna($sql);
    } else {
        $usuarioResp = new UsuarioResponsabilidade();
        $arEmpid = $usuarioResp->pegaEmpidPermitido($cpf);
    }

    return $arEmpid;
}

function possuiPerfilGestorUnidade($cpf)
{
    global $db;
    $pflcods = array((integer)PFLCOD_GESTOR_UNIDADE);
    if (count($pflcods) == 0) {
        return false;
    }
    $sql = "
	select
	count(*)
	from seguranca.perfilusuario
	where
	usucpf = '" . $_SESSION['usucpf'] . "' and
	pflcod in ( " . implode(",", $pflcods) . " ) ";
    return $db->pegaUm($sql) > 0;
}

function exibirHistoricoSigef($processo, $montaTabela = true)
{
    global $db;
    
    $randon = rand();

    $arrParam = array(
        'wsusuario' => 'USAP_WS_SIGARP',
        'wssenha' => '03422625',
        'nu_processo' => $processo,
        'method' => 'historicoempenho',
    );
    // Recuperando dados Empenho
    $empenho = montaXMLHistoricoProcessoSIGEF($arrParam);
    $empenho = $empenho ? $empenho : array();
	
    $dadosEmpenho = array();
    $cabecalhoEmpenho = array('Número da NE', 'Data do Empenho', 'Valor da NE', 'Espécie Empenho', 'Processo', 'CNPJ', 'Situação');
    
    $html = '<table class="listagem hstempsigef'.$randon.'" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
			<thead>
				<tr>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Número da NE</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Número da NE Pai</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Data do Empenho</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor da NE</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Espécie Empenho</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Processo</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">CNPJ</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Situação</td>
				</tr>
			</thead>';
    
    $valorempenhado = 0;
    $valorcancelado = 0;
    if( $empenho ){
    foreach($empenho as $count => $emp){

		$especieEmpenho = $db->pegaUm("select e.teecodigo||' - '||e.teedescricao from execucaofinanceira.tipoespecieempenho e where e.teecodigo = '{$emp['cod_especie']}'");
		
		if( $emp['cod_especie'] == '01' || $emp['cod_especie'] == '02' ){
			$especieEmpenho = '<span style="font-weight: bold; color: blue;">'.$especieEmpenho.'</span>';
			$valorempenhado += ($emp['valor_da_ne'] ? $emp['valor_da_ne'] : '0');
		} else {
			$especieEmpenho = '<span style="font-weight: bold; color: red;">'.$especieEmpenho.'</span>';
			$valorcancelado += ($emp['valor_da_ne'] ? $emp['valor_da_ne'] : '0');
		}
		
        /* $dadosEmpenho[$count][] = $emp['ano_do_empenho'] . 'NE' . $emp['numero_da_ne'];
        $dadosEmpenho[$count][] = formata_data($emp['data_do_empenho']);
        $dadosEmpenho[$count][] = number_format($emp['valor_da_ne'], 2, ',', '.');
        $dadosEmpenho[$count][] = $especieEmpenho;
        $dadosEmpenho[$count][] = $emp['numero_do_processo'].'&nbsp;';
        $dadosEmpenho[$count][] = $emp['cnpj'].'&nbsp;';
        $dadosEmpenho[$count][] = $emp['situacao_do_empenho']; */
        
        $key % 2 ? $cor = "#dedfde" : $cor = "";
        
        $html.= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td valign="middle">'.$emp['ano_do_empenho'] . 'NE' . $emp['numero_da_ne'].'</td>
					<td valign="middle">'.( $emp['numero_de_vinculacao_ne'] ? $emp['ano_do_empenho'] . 'NE' . $emp['numero_de_vinculacao_ne'] : '').'</td>
					<td valign="middle">'.formata_data($emp['data_do_empenho']).'</td>
                    <td valign="middle" align="right" style="color:#999999;">'.simec_number_format($emp['valor_da_ne'], 2, ',', '.').'</td>
					<td valign="middle">'.$especieEmpenho.'</td>
					<td valign="middle">'.$emp['numero_do_processo'].'</td>
					<td valign="middle">'.$emp['cnpj'].'</td>
					<td valign="middle">'.$emp['situacao_do_empenho'].'</td>
				';
    }
    $html .= '</tbody>
			</table>
			<table class="listagem hstempsigef'.$randon.'" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">    
				<tfoot>
					<tr>
						<td align="right" width="20%"><b>Total Empenhado:</b></td>
						<td align="left"><span style="font-weight: bold; color: blue;">'.simec_number_format($valorempenhado, '2', ',', '.').'</span></td>
                    </tr>
					<tr>
						<td align="right"><b>Total Cancelado:</b></td>
						<td align="left"><span style="font-weight: bold; color: red;">'.simec_number_format($valorcancelado, '2', ',', '.').'</span></td>
					</tr>
					<tr>
						<td align="right"><b>Total:</b></td>
						<td align="left"><span style="font-weight: bold; color: blue;">'.simec_number_format(($valorempenhado - $valorcancelado), '2', ',', '.').'</span></td>
					</tr>
				</tfoot>
               </table>';
    } else {
    	$html.= '<tr><td align="center" style="color:#cc0000;" colspan="8">Não foram encontrados Registros de Empenho.</td></tr></table>';
    }
    // Recuperando dados Pagamento
    $arrParam['method'] = 'historicopagamento';
    $pagamento = montaXMLHistoricoProcessoSIGEF($arrParam);
    $pagamento = $pagamento ? $pagamento : array();
	
    $dadosPagamento = array();
    $cabecalhoPagamento = array('Número da NE', 'Sequencial', 'Numero OB', 'Ano Exercício', 'Parcela', 'Valor da Parcela', 'Dt. Movimento', 'Dt. Emissão', 'Situação');
    foreach($pagamento as $count => $pag){
        $dadosPagamento[$count][] = $pag['nu_documento_siafi_ne'].'&nbsp;';
        $dadosPagamento[$count][] = $pag['nu_seq_mov_pag'].'&nbsp;';
        $dadosPagamento[$count][] = $pag['an_exercicio'].'OB'.$pag['nu_documento_siafi'].'&nbsp;';
        $dadosPagamento[$count][] = $pag['an_exercicio'].'&nbsp;';
        $dadosPagamento[$count][] = $pag['nu_parcela'].'&nbsp;';
        $dadosPagamento[$count][] = simec_number_format($pag['vl_parcela'], 2, ',', '.');
        $dadosPagamento[$count][] = formata_data($pag['dt_movimento']);
        $dadosPagamento[$count][] = formata_data($pag['dt_emissao']);
        $dadosPagamento[$count][] = $pag['ds_situacao_doc_siafi'];
    }
?>
<script>
jQuery(document).ready(function(){
	jQuery('#btn_mostra_hstempsigef<?=$randon ?>').live('click',function(){
		if( jQuery(this).attr('src') == '../imagens/menos.gif' ){
			jQuery('.hstempsigef<?=$randon ?>').hide();
			jQuery(this).attr('src','../imagens/mais.gif');
			jQuery(this).attr('title','Mostrar');
		}else{
			jQuery('.hstempsigef<?=$randon ?>').show();
			jQuery(this).attr('src','../imagens/menos.gif');
			jQuery(this).attr('title','Esconder');
		}
	});
	jQuery('#btn_mostra_hstpagsigef<?=$randon ?>').live('click',function(){
		if( jQuery(this).attr('src') == '../imagens/menos.gif' ){
			jQuery('.hstpagsigef<?=$randon ?>').hide();
			jQuery(this).attr('src','../imagens/mais.gif');
			jQuery(this).attr('title','Mostrar');
		}else{
			jQuery('.hstpagsigef<?=$randon ?>').show();
			jQuery(this).attr('src','../imagens/menos.gif');
			jQuery(this).attr('title','Esconder');
		}
	});
	jQuery('.hstempsigef<?=$randon ?>').hide();
	jQuery('.hstpagsigef<?=$randon ?>').hide();
});
</script>
<?php 
    if($montaTabela){
        echo '<table style="margin-top: 10px;" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1"><tr><td>';
    }

    // Exibindo dados Empenho
    echo '
		<div style="text-align: left;font-size:12px;" class="TituloTela">
			<img align="absmiddle" id="btn_mostra_hstempsigef'.$randon.'" style="cursor:pointer;" title="Mostrar" src="../imagens/mais.gif">
			Histórico de Empenho SIGEF
		</div>';
    echo $html;
    
  //  $db->monta_lista_simples($dadosEmpenho, $cabecalhoEmpenho,1000000,5,'S','100%', 'S', '', '', '', true);

    // Exibindo dados Pagamento
    echo '
		<div style="margin-top: 20px; text-align: left; font-size:12px;" class="TituloTela">
			<img align="absmiddle" id="btn_mostra_hstpagsigef'.$randon.'" style="cursor:pointer;" title="Mostrar" src="../imagens/mais.gif">
			Histórico de Pagamento SIGEF
		</div>
		<div class="hstpagsigef'.$randon.'">';
    $db->monta_lista_simples($dadosPagamento, $cabecalhoPagamento,1000000,5,'S','100%', 'S', '', '', '', true);
	echo '</div>';

    if($montaTabela){
        echo '</td></tr></table>';
    }
    
    return array(
				'empenho' => $empenho,
				'pagamento' => $pagamento
				);
}

function getObrasReformulacaoMunicipio ($aMunicipio){

    global $db;
    if(!empty($aMunicipio)) {
        $aMunicipio = (array)$aMunicipio;
        $sqlObrasAviso = "
				SELECT
                    o.obrid,
                    pre.preid,
                    o.obrnome,
                    pre.tooid,
                    sob.sbaid,
                    sob.sobano
                FROM
                    obras.preobra pre
                INNER JOIN workflow.documento 		doc ON doc.docid = pre.docid
                INNER JOIN par.instrumentounidade 	inu ON (inu.muncod = pre.muncodpar AND pre.tooid = 1) OR (inu.estuf = pre.estufpar AND pre.tooid <> 1)
                INNER JOIN obras2.obras o ON o.preid = pre.preid AND o.obrstatus = 'A' AND o.obridpai IS NULL
                INNER JOIN obras2.empreendimento e ON e.empid = o.empid
                LEFT JOIN entidade.endereco ed ON ed.endid = o.endid
                LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
                LEFT  JOIN par.subacaoobra			sob ON sob.preid = pre.preid

                WHERE
                    doc.esdid IN (1486, 1488) AND mun.muncod IN ('" . implode("', '", $aMunicipio) . "')
				";
        return $db->carregar($sqlObrasAviso);
    }
    return array();
}

/**
 *
 * @param string $cpf
 * @return array - Obras pendentes
 */
function getObrasReformulacaoPorCpf($cpf){
    $arEmpid 	 = pegaEmpidPermitido($cpf);

    $empreendimento = new Empreendimento();
    $municipios = $empreendimento->pegaMuncodPorEmpid($arEmpid);

    $esfera = verificaEsferaEmpreendimento($arEmpid);

    $aMunicipio = array();
    $aEstado 	= array();
    if(is_array($municipios)){
        if( $esfera == 'E' ){
            return array();
        } else {
            foreach($municipios as $municipio){
                $aMunicipio[] = $municipio['muncod'];
            }
            return getObrasReformulacaoMunicipio($aMunicipio);
        }
    }

}

function getObrasSolicitacaoDiligenciaPorCpf($cpf) {
    $arEmpid 	 = pegaEmpidPermitido($cpf);

    $empreendimento = new Empreendimento();
    $municipios = $empreendimento->pegaMuncodPorEmpid($arEmpid);
    $esfera = verificaEsferaEmpreendimento($arEmpid);
    $aMunicipio = array();
    
    if(is_array($municipios)) {
        if( $esfera == 'M' ) {
            foreach($municipios as $municipio) {
                $aMunicipio[] = $municipio['muncod'];
            }
            return getSolicitacoesDiligencia($aMunicipio);
        }
    }
}

function getSolicitacoesDiligencia($aMunicipio) {
    global $db;
    if(!$aMunicipio){
        return '';
    }
    $aMunicipio = is_array($aMunicipio) ? (implode("', '", $aMunicipio)) : $aMunicipio;
    $diligencia = ESDID_SOLICITACOES_DILIGENCIA;
    $sql = <<<DML
        SELECT count(0) FROM obras2.obras
        WHERE obrid IN(
            (SELECT obrid FROM obras2.obras obr
            WHERE endid IN
                ((SELECT DISTINCT endid FROM entidade.endereco WHERE muncod IN ('$aMunicipio')))
                AND
                ((SELECT true FROM obras2.solicitacao sol INNER JOIN workflow.documento doc ON sol.docid = doc.docid AND doc.esdid = $diligencia
                    WHERE sol.obrid = obr.obrid
                    LIMIT 1))
            ))
DML;
    return $db->pegaUm($sql);
}

function getObrasSolicitacaoDiligenciaPorMunicipio($aMunicipio) {
    return getSolicitacoesDiligencia($aMunicipio);
}

?>