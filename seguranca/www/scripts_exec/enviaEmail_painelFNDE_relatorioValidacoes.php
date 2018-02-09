<?php 
ini_set('memory_limit', '3024M');
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../'));

$_REQUEST['baselogin']  = 'simec_espelho_producao';//simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . '/global/config.inc';
require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/human_gateway_client_api/HumanClientMain.php';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';

$db = new cls_banco();

include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . 'www/autoload.php';
include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'includes/classes/modelo/obras2/Email.class.inc';
include_once APPRAIZ . 'includes/classes/modelo/obras2/AnexoEmail.class.inc';
include_once APPRAIZ . 'includes/classes/modelo/obras2/DestinatarioEmail.class.inc';
//include_once APPRAIZ . 'includes/classes/modelo/obras2/GerarExcelObras.class.inc';
include_once APPRAIZ . 'par/classes/modelo/Estado.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

/**
 * Retornar matriz com os dados Painel FNDE
 * @return array
 */
function pegaDadosPainelFNDE() {
    $arrSituacao = montaPainelSituacaoEstado($_POST);
    $totcolunas = array();

    if ($arrSituacao[0]) {
        foreach ($arrSituacao as $keys => $sit) {

            if (array_key_exists('esdid', $arrSituacao[$keys])) {
                unset($arrSituacao[$keys]['esdid']);
            }

            foreach ($arrSituacao[$keys] as $key => $col) {
                $totcolunas[$key] += $col;
            }

            $totcolunas['situacaoobra'] = 'TOTAL';
        }
        array_push($arrSituacao, $totcolunas);
    }
    
    return $arrSituacao;
}

/**
 * Retorna matriz com os dados do Relatório validações
 * @global cls_banco $db
 * @return array
 */
function pegaDadosRelatorioValidacoes() {
    global $db;
    $strSQL = "SELECT DISTINCT
		    oi.obrid,
		    oi.preid,
		    ent.entnome,
                     '(' || oi.obrid || ') ' || oi.obrnome as obra, 
            b.dcoconvenio as nr_convenio,
            Replace(Replace(Replace(oi.obrnumprocessoconv,'.',''),'/',''),'-','') AS proc,
            esd.esddsc,
		          
                    mun.mundescricao as municipio,
                    edo.estuf as uf,
                    COALESCE(oi.obrvalorprevisto, 0.00) as obrvalorprevisto,
                        COALESCE(oi.obrpercentultvistoria, 0.00) as obrpercexec,
                        CASE WHEN oi.preid IS NOT NULL THEN
                                'Sim'
                        else
                                'Não'
                        END as pagamento,
                        pag.pagvalorparcela,
                        pag.probanco || ' ' AS probanco,
                        pag.proagencia || ' ' AS proagencia,
                        pag.nu_conta_corrente || ' ' AS nu_conta_corrente,
                        v.vldstatushomologacao,
                    CASE WHEN (v.vldobshomologacao IS NULL or v.vldobshomologacao = '') THEN 'Sem observações' ELSE v.vldobshomologacao END as observacao,
                    v.vldstatus25exec,
                    v.vldstatus50exec,
		    too.toodescricao,
		    pf.prfdesc,
		    u.usunome AS usunome_hom,
		    TO_CHAR(v.vlddtinclusaosthomo, 'DD-MM-YYYY') AS data_hom,
		    u1.usunome AS usunome_25,
		    TO_CHAR(v.vlddtinclusaost25exec, 'DD-MM-YYYY') AS data_25,
		    u2.usunome AS usunome_50,
		    TO_CHAR(v.vlddtinclusaost50exec, 'DD-MM-YYYY') AS data_50
		FROM
                    obras2.empreendimento ep 

		INNER JOIN obras2.obras oi ON oi.empid = ep.empid 
		LEFT JOIN workflow.documento doc ON doc.docid = oi.docid 
			
		LEFT JOIN (SELECT
                                SUM(p.pagvalorparcela) AS pagvalorparcela,
                                probanco,
                                proagencia,
                                nu_conta_corrente,
                                emo.preid
                           FROM
                                par.empenhoobra emo
                                INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' 
                                INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A' 						
                                INNER JOIN par.pagamento p ON p.empid = emo.empid AND 
                                p.pagstatus='A'
                           GROUP BY
                                emo.preid, pro.probanco, pro.proagencia, pro.nu_conta_corrente) pag ON pag.preid = oi.preid
		    LEFT JOIN painel.dadosconvenios b           ON b.dcoprocesso = Replace(Replace(Replace(oi.obrnumprocessoconv,'.',''),'/',''),'-','')
		    LEFT JOIN entidade.entidade                ent ON ent.entid  = ep.entidunidade
		    LEFT JOIN entidade.endereco                 ed ON ed.entid   = ent.entid
		    LEFT JOIN entidade.endereco                edo ON edo.endid  = oi.endid
		    LEFT JOIN territorios.municipio                 mun ON mun.muncod = ed.muncod
		    LEFT JOIN obras2.validacao                   v ON v.obrid    = oi.obrid
		    LEFT JOIN seguranca.usuario                  u ON u.usucpf   = v.usucpf_homo
		    LEFT JOIN seguranca.usuario                 u1 ON u1.usucpf  = v.usucpf_25
		    LEFT JOIN seguranca.usuario                 u2 ON u2.usucpf  = v.usucpf_50
		    LEFT JOIN obras2.tipoorigemobra            too ON too.tooid  = oi.tooid
		    LEFT JOIN obras2.programafonte              pf on pf.prfid   = ep.prfid
                    LEFT JOIN workflow.documento                 d ON d.docid    = oi.docid
                    LEFT JOIN workflow.estadodocumento         esd ON esd.esdid  = d.esdid
                    LEFT JOIN painel.dadosconvenios       dco ON dco.dcoprocesso = Replace(Replace(Replace(oi.obrnumprocessoconv,'.',''),'/',''),'-','')
		WHERE
                    oi.obrstatus = 'A' AND ep.empstatus = 'A' AND oi.obridpai IS NULL AND ep.orgid = 3 order by oi.obrid";

    return $db->carregar($strSQL);
}

$arrSituacao = pegaDadosPainelFNDE();
$cabecalho = array('Situação Obra', 'AC', 'AL', 'AM', 'AP', 'BA', 'CE', 'DF', 'ES', 'GO', 'MA', 'MG', 'MS', 'MT', 'PA', 'PB', 'PE', 'PI', 'PR', 'RJ', 'RN', 'RO', 'RR', 'RS', 'SC', 'SE', 'SP', 'TO', 'Totoal');
$relPainelFNDE = _sql_to_excel($arrSituacao, 'relPainelFNDE', $cabecalho, '', 'F');

$arrValidacoes = pegaDadosRelatorioValidacoes();
$cabecalho = array('obrid','preid','entnome','obra','nr_convenio', 'proc', 'esddsc','municipio','uf','obrvalorprevisto', 'obrpercexec', 'pagamento', 'pagvalorparcela', 'probanco', 'proagencia', 'nu_conta_corrente', 'vldstatushomologacao','observacao', 'vldstatus25exec', 'vldstatus50exec', 'toodescricao', 'prfdesc', 'usunome_hom', 'data_hom', 'usunome_25', 'data_25', 'usunome_50', 'data_50');
$relValidacoes = _sql_to_excel($arrValidacoes, 'relValidacoes', $cabecalho, '', 'F');
//$relValidacoes = _sql_to_csv($arrValidacoes);

$file = new FilesSimec(null, null, 'obras2');
$file->setPasta('obras2');
$arqid1 = $file->setStream('painelFNDE', $relPainelFNDE, 'application/excel', '.xls', false, 'painelFNDE.xls');
$arqid2 = $file->setStream('relatorioValidacoes', $relValidacoes, 'application/excel', '.xls', false, 'relatorioValidacoes.xls');

$bytes1 = filesize($file->getCaminhoFisicoArquivo($arqid1));
$bytes2 = filesize($file->getCaminhoFisicoArquivo($arqid2));

function FileSizeConvert($bytes) {
    $bytes = floatval($bytes);
        $arBytes = array(
            0 => array(
                'UNIT' => 'TB',
                'VALUE' => pow(1024, 4)
            ),
            1 => array(
                'UNIT' => 'GB',
                'VALUE' => pow(1024, 3)
            ),
            2 => array(
                'UNIT' => 'MB',
                'VALUE' => pow(1024, 2)
            ),
            3 => array(
                'UNIT' => 'KB',
                'VALUE' => 1024
            ),
            4 => array(
                'UNIT' => 'B',
                'VALUE' => 1
            ),
        );

    foreach($arBytes as $arItem) {
        if($bytes >= $arItem['VALUE']) {
            $result = $bytes / $arItem['VALUE'];
            $result = str_replace('.', ',' , strval(round($result, 2))).' '.$arItem['UNIT'];
            break;
        }
    }
    return $result;
}

$email = new Email();
$email->enviaEmailPainelFNDE_RelatorioValidacoes(array($arqid1, $arqid2));
$email->enviar();

//echo '<h1>Concluido !</h1>';
//echo FileSizeConvert($bytes1).'<br>';
//echo FileSizeConvert($bytes2).'<br>';
//exit;