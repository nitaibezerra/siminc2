<?php

function monta_agp_relatorio_infra()
{
    $agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];

    $agp = array(
        "agrupador" => array(),
        "agrupadoColuna" => array(
            "audpercequipada",
            "audmobiliada",
            "audqtd",
            "unidademedida"
        ),
        "agrupadorDetalhamento" => array(
            array(
                "campo" => "audpercequipada",
                "label" => "aaaaaaaaa"
            ),


            array(
                "campo" => "nomedaobra",
                "label" => "Nome da Obra"
            )
        )
    );

    //audqtd,audmobiliada,audpercequipada

    foreach ($agrupador as $val) {
        switch ($val) {
            case "campus":
                array_push($agp['agrupador'], array(
                        "campo" => "campus",
                        "label" => "Campus")
                );
                break;
            case "modulo":
                array_push($agp['agrupador'], array(
                        "campo" => "modulo",
                        "label" => "Módulo")
                );
                break;
            case "ambiente":
                array_push($agp['agrupador'], array(
                        "campo" => "ambiente",
                        "label" => "Ambiente")
                );
                break;
            case "uf":
                array_push($agp['agrupador'], array(
                        "campo" => "uf",
                        "label" => "UF")
                );
                break;
            case "unidade":
                array_push($agp['agrupador'], array(
                        "campo" => "unidade",
                        "label" => "Unidade")
                );
                break;

        }
    }
    return $agp;
}

function monta_coluna_relatorio_infra()
{
    $coluna = array();

    if (in_array("nomedaobra", $_REQUEST['agrupador'])) {
        $_REQUEST['colunas'] = $_REQUEST['colunas'] ? $_REQUEST['colunas'] : array();
        foreach ($_REQUEST['colunas'] as $valor) {
            switch ($valor) {
                case 'usucpfexclusao':
                    array_push($coluna, array("campo" => "usucpfexclusao",
                        "label" => "Resposansável pela Exclusão",
                        "blockAgp" => "",
                        "type" => "string"));
                    break;
            }
        }
    }
    array_push($coluna, array("campo" => "audqtd",
        "label" => "Quantidade",
        "blockAgp" => "audqtd",
        "type" => "numeric"));

    array_push($coluna, array("campo" => "audmobiliada",
        "label" => "Mobiliada",
        "blockAgp" => "audmobiliada",
        "type" => "string"));

    array_push($coluna, array("campo" => "audpercequipada",
        "label" => "Percentual de equipamento",
        "blockAgp" => "audpercequipada",
        "type" => "string"));

    array_push($coluna, array("campo" => "unidademedida",
        "label" => "Unidade de medida",
        "blockAgp" => "unidademedida",
        "type" => "string"));


    return $coluna;
}

function query_relatorio_infra(){

// monta o sql, agrupador e coluna do relatório
    $where = "";

//ver($_REQUEST['unidademed'], d);
    if (!empty($_REQUEST["unidade"][0])) {
        $unidades = implode(",", $_REQUEST["unidade"]);
        $where .= " and ua.entidunidade in($unidades)";
    }


    if (!empty($_REQUEST["campus"][0])) {
        $campus = implode(",", $_REQUEST["campus"]);
        $where .= " and ua.entidcampus in($campus)";
    }

    if (!empty($_REQUEST["modulos"][0])) {
        $modulo = implode(",", $_REQUEST["modulo"]);
        $where .= " and m.mdlid in($modulo)";
    }

    if (!empty($_REQUEST["ambiente"][0])) {
        $ambiente = implode(",", $_REQUEST["ambiente"]);
        $where .= " and a.ambid in($ambiente)";
    }

    if (!empty($_REQUEST["unidademed"][0])) {
        $unidademed = implode(",", $_REQUEST["unidademed"]);
        $where .= " and ua.entidcampus in($unidademed)";
    }

    if (!empty($_REQUEST["mobiliada"])) {

        if ($_REQUEST["mobiliada"] == "Sim") {
            $mobiliada = 't';
            $where .= " and audmobiliada = '$mobiliada'";
        }
        if ($_REQUEST["mobiliada"] == "Não") {
            $mobiliada = 'f';
            $where .= " and audmobiliada = '$mobiliada'";
        }
    }



            $percentualinicial = $_REQUEST["percentualinicial"];
            $percentualfinal = $_REQUEST["percentualfinal"];
            $where .= " and (audpercequipada >= {$percentualinicial} and audpercequipada <= {$percentualfinal} ) ";






    if (!empty($_REQUEST["orgid"][0]) || !empty($_REQUEST["orgid"][1]) || !empty($_REQUEST["orgid"][2])) {
        $i = 0;
        $where .="and ( 1=1 and ";
        if (!empty($_REQUEST["orgid"][0])) {
            $where .= "ef.funid in ('" . ACA_ID_UNIVERSIDADE . "')";
            $i++;
        }
        if (!empty($_REQUEST["orgid"][1])) {
            if($i == 1){
                $where .= ' or ';
            }
            $i++;
            $where .= "ef.funid in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";

        }
        if (!empty($_REQUEST["orgid"][2])) {
            if($i == 1 || $i == 2){
                $where .= ' or ';
            }
            $where .= "ef.funid in ('" . ACA_ID_UNIDADES_VINCULADAS . "')";
        }
        $where .= ")";

    }

    $sql = "select DISTINCT audid,ua.entidcampus, ua.entidunidade, ua.entidcampus,mdldsc as modulo,ambdsc as ambiente,m.mdlid,audqtd,audpercequipada,auddsc,a.ambid,
        (select entnome from entidade.entidade where entid = ua.entidcampus) as campus,
        (select entnome from entidade.entidade where entid = ua.entidunidade) as unidade,
        (select mun.estuf FROM
					entidade.entidade e2
				LEFT JOIN
					entidade.endereco ed ON ed.entid = e2.entid
				LEFT JOIN
					territorios.municipio mun ON mun.muncod = ed.muncod
                WHERE e2.entid = ua.entidunidade
                ) AS UF,

        case when audmobiliada = 't' then 'Sim' when audmobiliada = 'f' then 'Não'  END as audmobiliada,
        unddsc as unidademedida
        from academico.modulo m
	left join academico.moduloambiente ma on m.mdlid = ma.mdlid
	left join academico.ambiente a on a.ambid = ma.ambid
	left join academico.unidadeambiente ua on  ua.ambid = a.ambid and ma.mdlid = ua.mdlid
	left join academico.unidademed um on  ma.undid =um.undid
	LEFT JOIN 	entidade.funcaoentidade ef ON ef.entid = ua.entidunidade
	where entidcampus >= 1 $where
     order by mdlid";

    return $sql;
}



?>