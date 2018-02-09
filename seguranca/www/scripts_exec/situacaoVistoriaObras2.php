<?php

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';


$db = new cls_banco();


include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/ItensComposicaoObras.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/ObrasContrato.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Supervisao.class.inc";
include_once APPRAIZ . "includes/workflow.php";

    if(!$_REQUEST['obrid'])
        $obrid = null;
    else
        $obrid = (is_array($_REQUEST['obrid'])) ? implode(',', $_REQUEST['obrid']) : implode(',', explode(',', $_REQUEST['obrid']));

    $obrid = (!$obrid) ? " (f.supdtinclusao > f.htddata) " : " f.obrid IN ($obrid)";

    $sqlObras = "


           SELECT * FROM (
            SELECT

                  o.obrid,
                  o.docid,
                  d.esdid,
                  (SELECT
                  s.staid
                   FROM
                     obras2.supervisao s
                   WHERE
                     s.obrid = o.obrid AND
                     s.emsid IS NULL AND s.smiid IS NULL AND
                     s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S'
                     AND s.rsuid = 1
                   ORDER BY
                     s.supdata DESC, s.supdtinclusao DESC, s.supid DESC LIMIT 1) as staid,
                   (SELECT
                  s.supid
                   FROM
                     obras2.supervisao s
                   WHERE
                     s.obrid = o.obrid AND
                     s.emsid IS NULL AND s.smiid IS NULL AND
                     s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S'
                     AND s.rsuid = 1
                   ORDER BY
                     s.supdata DESC, s.supdtinclusao DESC, s.supid DESC LIMIT 1) as supid,
                   (SELECT
                  s.supdtinclusao
                   FROM
                     obras2.supervisao s
                   WHERE
                     s.obrid = o.obrid AND
                     s.emsid IS NULL AND s.smiid IS NULL AND
                     s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S'
                     AND s.rsuid = 1
                   ORDER BY
                     s.supdata DESC, s.supdtinclusao DESC, s.supid DESC LIMIT 1)  as supdtinclusao,

                  (SELECT htddata FROM workflow.historicodocumento WHERE docid = d.docid ORDER BY htddata DESC LIMIT 1) as htddata

                FROM obras2.obras o
                  JOIN workflow.documento d ON d.docid = o.docid
                  JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                WHERE
                  o.obrstatus = 'A' AND
                  obridpai IS NULL AND
                  --o.obridvinculado IS NULL AND
                  o.obrid IN (  SELECT s.obrid FROM
                                     obras2.supervisao s
                                    WHERE
                                      s.emsid IS NULL AND s.smiid IS NULL AND
                                      s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S'
                                      AND s.rsuid = 1
                                    GROUP BY s.obrid)
                  AND d.esdid IN(690, 691, 693) AND
                  staid IN (1,2,3)

        ) as f WHERE  ((f.staid = 1 AND f.esdid != 690) OR (f.staid = 3 AND f.esdid != 693) OR (f.staid = 2 AND f.esdid != 691)) AND ($obrid)


      ";

    $obras = $db->carregar($sqlObras);

    if(empty($obras)) exit;
    foreach($obras as $obra){

        $destino  = null;
        switch ( $obra['staid']) {
            case 1: // Execução
                $destino = 690;
                break;
            case 2: // Paralisado
                $destino = 691;
                break;
            case 3: // Concluido
                $destino = 693;
                break;
        }

        if(wf_acaoPossivel($obra['docid'], $destino, array('obrid' => $obra['obrid']))){
            $acao = wf_pegarAcao( $obra['esdid'], $destino);

            $supervisao = new Supervisao($obra['supid']);

            $sqlHistorico = "
                insert into workflow.historicodocumento
                ( aedid, docid, usucpf, htddata )
                values ( " . $acao['aedid'] . ", " . $obra['docid'] . ", '" . $supervisao->usucpf . "', '".$supervisao->supdata."' )
                returning hstid
            ";
            $hstid = (integer) $db->pegaUm( $sqlHistorico );

            $sqlDocumento = "
                update workflow.documento
                set esdid = " . $acao['esdiddestino'] . ", hstid = $hstid
                where docid = " . $obra['docid'];
            $db->executar( $sqlDocumento );

            $db->commit();

        }
    }
    ver($obras, d);
