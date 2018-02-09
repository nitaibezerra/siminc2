<?php
ini_set( 'display_errors', 1 );
ini_set("memory_limit", "9024M");
ini_set("default_socket_timeout", "70000000");

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao";//simec_desenvolvimento
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
include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Validacao.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";


geraRelatorioEvolucao();
geraRelatorioValidacao();

echo 'Arquivos criados com sucesso!';

exit;

function geraRelatorioEvolucao()
{
    global $db;
    $dir_relatorios = APPRAIZ . 'arquivos/obras2/relatorio/evolucaomi';

    if(!file_exists($dir_relatorios))
        mkdir($dir_relatorios, 0777, true);

    $dir_relatorios .= '/';

    $sql = "SELECT
                    o.obrid,
                    o.obrnome,
                    pf.prfdesc as programa,
                    too.toodescricao as fonte,
                    ed.esddsc as situacao,
                    mun.estuf,
                    mun.mundescricao,


                    DATE_PART('days', (SELECT s.supdata
					FROM obras2.obras o1
						    JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
						    WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
						    ORDER BY supdata DESC LIMIT 1)
							-
					(SELECT s.supdata
					FROM obras2.obras o1
						    JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
						    WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
						    ORDER BY supdata DESC OFFSET 2 LIMIT 1) ) as days_ult_vist,



                    (SELECT (SELECT
                                     CASE
                                         WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                         ELSE 0::numeric
                                     END AS total
                                      FROM obras2.itenscomposicaoobra i
                                        INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                        LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                      WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC LIMIT 1)
                                        -
                                (SELECT (SELECT
                                         CASE
                                             WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                             ELSE 0::numeric
                                         END AS total
                                          FROM obras2.itenscomposicaoobra i
                                          INNER JOIN obras2.cronograma cro ON cro.croid = i.croid  AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                            LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                          WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 2 LIMIT 1) as percent_ult_vist,

                    DATE_PART('days', (SELECT s.supdata

                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 3 LIMIT 1)
                                        -
                                (SELECT s.supdata
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 5 LIMIT 1) ) as days_pen_vist,



                    (SELECT (SELECT
                                     CASE
                                         WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                         ELSE 0::numeric
                                     END AS total
                                      FROM obras2.itenscomposicaoobra i
                                      INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus = 'A'
                                        LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                      WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 3 LIMIT 1)
                                        -
                                (SELECT (SELECT
                                         CASE
                                             WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                             ELSE 0::numeric
                                         END AS total
                                          FROM obras2.itenscomposicaoobra i
                                          INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                            LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                          WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 5 LIMIT 1) as percent_pen_vist,



                  DATE_PART('days', (SELECT s.supdata
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 6 LIMIT 1)
                                        -
                                (SELECT s.supdata
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 8 LIMIT 1) ) as days_ant_vist,
                  (SELECT (SELECT
                                     CASE
                                         WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                         ELSE 0::numeric
                                     END AS total
                                      FROM obras2.itenscomposicaoobra i
                                      INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                        LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                      WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 6 LIMIT 1)
                                        -
                                (SELECT (SELECT
                                         CASE
                                             WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                             ELSE 0::numeric
                                         END AS total
                                          FROM obras2.itenscomposicaoobra i
                                          INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                            LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                          WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 8 LIMIT 1) as percent_ant_vist,


                    (SELECT ARRAY_TO_STRING(ARRAY(
                    SELECT
                            TO_CHAR(s.supdata, 'DD/MM/YYYY')::text || ';' ||
                            (SELECT
                                 CASE
                                     WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                     ELSE 0::numeric
                                 END AS total
                                  FROM obras2.itenscomposicaoobra i
                                  INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                    LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                  WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)::text

                    FROM obras2.obras o1
                    JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                    WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                    ORDER BY supdata DESC), '@')) as vistorias

            FROM obras2.obras o
            JOIN obras2.empreendimento          ep ON ep.empid   = o.empid
            LEFT JOIN workflow.documento       doc ON doc.docid  = o.docid
            LEFT JOIN workflow.estadodocumento 	ed ON ed.esdid   = doc.esdid
            LEFT JOIN obras2.tipoorigemobra    too ON too.tooid  = o.tooid
            LEFT JOIN obras2.programafonte      pf ON pf.prfid   = ep.prfid
            LEFT JOIN entidade.endereco        edo ON edo.endid  = o.endid
            LEFT JOIN territorios.municipio    mun ON mun.muncod = edo.muncod
            WHERE o.obridpai IS NULL
              AND o.obrstatus = 'A' ";

    $obras = $db->carregar($sql);
    $cabecalho = array("ID", "Nome", "Programa", "Fonte","Situação","UF", "Município", "Vistorias");

    array_pop($cabecalho);
    $maxVst = 0;
    foreach ($obras as $key => $obra) {
        $data = $obras[$key]['vistorias'];
        $vistorias = str_replace('@', ';', $obras[$key]['vistorias']);
        unset($obras[$key]['vistorias']);
        $arV = explode(';', $vistorias);
        $maxVst = (count($arV) > $maxVst) ? count($arV) : $maxVst;

        $qtd = 0;
        $vistorias = explode('@', $data);
        foreach($vistorias as $v){
            $qtd++;
            $d = explode(';',$v);
            $obras[$key][] = $d[0];
            $obras[$key][] = $d[1];

            switch ($qtd){
                case 3:
                    $obras[$key][] = $obras[$key]["days_ult_vist"];
                    $obras[$key][] = $obras[$key]["percent_ult_vist"];
                    break;
                case 6:
                    $obras[$key][] = $obras[$key]["days_pen_vist"];
                    $obras[$key][] = $obras[$key]["percent_pen_vist"];
                    break;
                case 9:
                    $obras[$key][] = $obras[$key]["days_ant_vist"];
                    $obras[$key][] = $obras[$key]["percent_ant_vist"];
                    break;
            }
        }
        unset($obras[$key]["days_ant_vist"]);
        unset($obras[$key]["percent_ant_vist"]);
        unset($obras[$key]["days_ult_vist"]);
        unset($obras[$key]["percent_ult_vist"]);
        unset($obras[$key]["days_pen_vist"]);
        unset($obras[$key]["percent_pen_vist"]);
    }

    for ($x = 1; $x <= $maxVst; $x++){
        if(($x % 2) != 0)
            $cabecalho[] = 'Dt. Vistoria';
        else
            $cabecalho[] = '% Vistoria';

        switch ($x){
            case 6:
                $cabecalho[] = "Qtd Dias entre as 3 últimas vistorias";
                $cabecalho[] = "% de avanço de obra nas 3 últimas vistorias";
                break;
            case 12:
                $cabecalho[] = "Qtd Dias entre as 3 penúltimas vistorias";
                $cabecalho[] = "% de avanço de obra nas 3 penúltimas vistorias";
                break;
            case 18:
                $cabecalho[] = "Qtd Dias entre as 3 anti-penúltimas vistorias";
                $cabecalho[] ="% de avanço de obra nas 3 anti-penúltimas vistorias";
                break;
        }
    }

// Limpa antigos
    $scanned_directory = array_diff(scandir($dir_relatorios), array('..', '.'));
    foreach ($scanned_directory as $file)
        unlink($dir_relatorios . $file);

    sql_to_xml_excel_file($obras, 'rel_evo_obrasmi', $cabecalho, $dir_relatorios);


}

function geraRelatorioValidacao()
{
    global $db;
    $dir_relatorios = APPRAIZ . 'arquivos/obras2/relatorio/validacao';

    if(!file_exists($dir_relatorios))
        mkdir($dir_relatorios, 0777, true);

    $dir_relatorios .= '/';

    $validacao = new Validacao();
    $sql = $validacao->listaRelatorioValidacao(array());

    $obras = $db->carregar($sql);
    $cabecalho = array( "Id Obra", "ID Pré-Obra", "Unidade Implantadora", "Nome da Obra", "Nº Termo/Convênio", "Processo", "Situação","Município", "UF", "Valor Previsto", "(%) Executado", "Pagamento", "Total Pago", "Banco", "Agência", "Conta", "Total da conta", "Data de referencia do SALDO \"MES/ANO\"", "Homologado", "Observações", "Execução 25%", "Observações", "Execução 50%", "Observações","CA","CV","CT","CO","Fonte", "Programa", "Tipologia", "Técnico da Homologação", "Data da Homologação", "Técnico da Execução 25%", "Data da Execução 25%", "Técnico da Execução 50%", "Data da Execução 50%", "Data do último pagamento", "Situação do último pagamento", "% Pago", "Fim Vigência da Obra", "Fim Vigência Termo", "início Vigência Termo");

    // Limpa antigos
    $scanned_directory = array_diff(scandir($dir_relatorios), array('..', '.'));
    foreach ($scanned_directory as $file)
        unlink($dir_relatorios . $file);

    sql_to_xml_excel_file($obras, 'rel_validacao', $cabecalho, $dir_relatorios);


}

function sql_to_xml_excel_file($sql, $nome_arquivo, array $cabecalho = array(), $dir)
{
    ob_clean();

    // este método transforma uma query em excel
    $extension = ".xls";
    $arquivo = "SIMEC_" . $nome_arquivo . "_" . date("d-m-Y_his") . $extension;


    $data = !is_array($sql) ? $this->carregar($sql) : $sql;
    $data = is_array($data) ? $data : array();
    array_unshift($data, $cabecalho);

    $xls = new Excel_XML;
    $xls->addWorksheet($nome_arquivo, $data);
    $xls->writeWorkbook($arquivo, $dir);

}