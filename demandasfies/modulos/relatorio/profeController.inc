<?php

require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaClassificacao.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaEntrega.class.inc";
include_once APPRAIZ . "demandasfies/classes/Classificacao.class.inc";
require_once APPRAIZ . "includes/mpdf60/mpdf.php";
ini_set('max_execution_time', 120);

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$modelDemandaClassificacao = new DemandaClassificacao();
$modelDemandaEntrega = new DemandaEntrega($_REQUEST['dmeid']);

$camposDaTabela = getCamposDaTabela();

$modelDemanda = new Demanda();
$modelDemanda->popularDadosObjeto();
$dmdstatusdecisao = $modelDemanda->dmdstatusdecisao;

$claid = $_POST['claid'];

$perfis = $_SESSION['perfilusuario'][$_SESSION['usucpf']];

if ($_POST['btn_relatorio'] == 'relatorio') {
    $dados = recuperarListagem($modelDemanda, $modelDemandaClassificacao);

    $html = setBody($dados, $camposDaTabela, 'pdf');
    html2Pdf($html);
}

if ($_POST['action'] == 'pesquisar') {
    $esdid_entrega = $_POST['esdid_entrega'];
    $area = $_POST['area'];
    $usucpf = $_POST['usucpf'];
}

include APPRAIZ . "includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');

$esdid = $_REQUEST['esdid'];
$areas = getAreas();
$pendecia = $_POST['pendecia'];
$dt_inicio = $_POST['dt_inicio'];
$dt_fim = $_POST['dt_fim'];

function html2Pdf($html)
{
    $img_file = APPRAIZ . 'www/imagens/brasao.gif';
    $imgData = base64_encode(file_get_contents($img_file));
    $src = 'data:' . mime_content_type($img_file) . ';base64,' . $imgData;

    $data = date('d/m/Y H:i:s');
    $topo = "<html>
		<head>
		<style type='text/css' media='print'>
		    @page
            {
                size: auto;
                margin: 3mm;
            }
            body
            {
                margin: 0px;
            }
			.text-left {
				text-align: left;
			}
			.text-right {
				text-align: right;
			}
			.text-center {
				text-align: center;
			}
			.text-justify {
				text-align: justify;
			}
			th {
				text-align: left;
			}
			table {
				border-collapse: collapse;
				border-spacing: 0;
				margin-bottom: 20px;
				max-width: 100%;
				width: 100%;
			    font-size: 7px;
				border: 1px solid #ddd;
			}
			td, th {
				padding: 3px;
				border: 1px solid #ddd;
				vertical-align: middle;
				border-bottom-width: 2px;
			}
			th {
				border-bottom: 2px solid #ddd;
				display: table-cell;
				text-align: center;
   				vertical-align: middle
			}
			table > caption + thead > tr:first-child > td, .table > caption + thead > tr:first-child > th, .table > colgroup + thead > tr:first-child > td, .table > colgroup + thead > tr:first-child > th, .table > thead:first-child > tr:first-child > td, .table > thead:first-child > tr:first-child > th {
				border-top: 0 none;
			}

			thead{
				text-align: center;
				background-color: #d5d5d5;
				vertical-align: middle;
			}
			.tabela-demanda td, {
				background-color: #f4f4f4;
			}
			.tabela-entrega {
				margin-left: 15px;;
				width: 99%;
			}
			.tabela-entrega td {
				background-color: #e3eaea;
			}
			.th_entrega {
				background-color: #cecece;
			}
			.th_demanda {
				background-color: #bebebe;
			}
		</style>
		</head>
		<body>
			<table width='100%' class='table table-condensed' style='font-size: 10px;'>
				<tr>
					<td class='text-center'>
						<img src='{$src}' width='60'>
					</td>
					<td>
						". NOME_SISTEMA. "<br>
						Ministério da Educação / SE - Secretaria Executiva<br>
						Sistema - Demandas FIES
					</td>
					<td>
						Impresso por: <b>{$_SESSION['usunome']}</b><br> Hora da Impressão:{$data}<br>
					</td>
				</tr>
			</table>
";
    $rodape = ' </body>	</html> ';
    $conteudo = $topo . $html . $rodape;
    $arqnome = "relatorio_demadas_fies_profe.pdf";


//    echo $conteudo;exit;


    $conteudo_html = http_build_query(array(
        'conteudoHtml' => utf8_encode($conteudo),
        'orientacao' => 'paisagem',
    ));
    $context = stream_context_create(array(
        'http' => array(
            'method' => 'POST',
            'content' => $conteudo_html
        )
    ));
    $contents = file_get_contents('http://wshmg.mec.gov.br/ws-server/htmlParaPdf', null, $context);
    header('Content-Type: application/pdf');
    header('Content-Disposition: attachment; filename=Relatorio_AE_');
    echo $contents;
    exit();
}

function setHeader($camposDaTabela, $tabelaArvore)
{
    $tabelaArvore->addTSection('thead');
    $class = 'thead';
    $tabelaArvore->addRow($class);
    foreach ($camposDaTabela as $campo) {
        $tabelaArvore->addCell($campo, '', 'header', array('class' => 'text-center',));
    }
}

function setHeaderEntrega($tabelaArvore, $tipoRelatorio = null)
{
    $camposDaTabela = getCamposDaTabelaEntrega();
    if ($tipoRelatorio == 'pdf') {
        unset($camposDaTabela['data_estado_entrega']);
        unset($camposDaTabela['dmerelocorrencia']);
    }

    $tabelaArvore->addTSection('thead');
    $class = 'thead';
    $tabelaArvore->addRow($class);
    foreach ($camposDaTabela as $campo) {
        $tabelaArvore->addCell($campo, '', 'header', array('class' => 'text-center th_entrega'));
    }
}

function setBody($dados, $camposDaTabela, $tipoRelatorio = null)
{
    $tabelaArvore = new HTML_Table('relaorio_pdf_demandas_fies', 'table table-condensed table-bordered  table-responsive');
    $colunasEscondidas = getColunasEscondidas();
    setHeader($camposDaTabela, $tabelaArvore);
    $tabelaArvore->addTSection('tbody');
    $html = '';

    $cont = 0;
    foreach ($dados as $key => $dado) {
        $cont++;

        $tabelaArvore->addRow();

        foreach ($colunasEscondidas as $colunaEscondida) {
            unset($dado[$colunaEscondida]);
        }

        foreach ($camposDaTabela as $atributo => $campo) {
            $class = 'text-center';

            if ($atributo == 'objnome') {
                $valor = strtolower($dado[$atributo]);
            } elseif ($atributo == 'autor') {
                $valor = ucwords(strtolower($dado[$atributo]));
            } elseif ($atributo == 'cont') {
                $valor = $cont;
            } else {
                $valor = $dado[$atributo];
            }
            $tabelaArvore->addCell($valor, $class);
        }

        if ($tipoRelatorio != 'pdf') {
            getTabelaEntrega($dado, $tabelaArvore, $tipoRelatorio);
        }

        if ($tipoRelatorio == 'pdf') {
            if ($cont % 25 == 0) {
                $html .= $tabelaArvore->display();
                $tabelaArvore = new HTML_Table('relaorio_pdf_demandas_fies' . $cont, 'table table-condensed table-bordered  table-responsive', array('style'=>'page-break-before: always;'));
                setHeader($camposDaTabela, $tabelaArvore);
                $tabelaArvore->addTSection('tbody');
            }
        }
    }

    if ($tipoRelatorio != 'pdf') {
        $html .= $tabelaArvore->display();
    }
    return $html;
}

function getTabelaEntrega($dado, $tabelaArvore, $tipoRelatorio = null)
{
    $camposDaTabelaEntrega = getCamposDaTabelaEntrega();

    if ($tipoRelatorio) {
        unset($camposDaTabelaEntrega['data_estado_entrega']);
        unset($camposDaTabelaEntrega['dmerelocorrencia']);
    }

    $class = '';
    if (is_array($dado['entrega'])) {
        $tabelaEntrega = '';
        $tabelaArvoreEntrega = new HTML_Table('relaorio_entrega_demandas_fies', "table table-bordered table-condensed tabela-listagem tabela-entrega", array('data-id-demanda' => $dado['codigo']));
        setHeaderEntrega($tabelaArvoreEntrega, $tipoRelatorio);
        $tabelaArvoreEntrega->addTSection('tbody');

        foreach ($dado['entrega'] as $entrega) {
            $tabelaArvoreEntrega->addRow();
            foreach ($camposDaTabelaEntrega as $atributo2 => $campo2) {
                $tabelaArvoreEntrega->addCell($entrega[$atributo2]);
            }
        }

        $tabelaEntrega = $tabelaArvoreEntrega->display();
        $tabelaArvore->addRow();
        $tabelaArvore->addCell($tabelaEntrega, 'td_entregas', 'data', array('colspan' => 13));

    }
}

function recuperarListagem(Demanda $modelDemanda, DemandaClassificacao $modelDemandaClassificacao, $comfitro = true)
{
    $sql = getSqlPesquisa($modelDemanda, $modelDemandaClassificacao, $comfitro);

    $dados = $modelDemanda->carregar($sql);
    $dados = $dados ? $dados : array();
    $demanda = organizarDemandasEntregas($dados);
    return $demanda;
}

function montarTabelaListagemProfe($modelDemanda, $modelDemandaClassificacao)
{
    $camposDaTabela = getCamposDaTabela();
    $colunasEscondidas = getColunasEscondidas();

    $sql = getSqlPesquisa($modelDemanda, $modelDemandaClassificacao);

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PDF);
    $listagem->setTamanhoPagina(100);
    $listagem->addCallbackDeCampo(array('objnome', 'autor', 'esddsc', 'dpanome'), 'alinhaParaEsquerda');
    $listagem->addCallbackDeCampo(array('acjnome'), 'tratarDemandaJudicialLista');
    $listagem->esconderColunas($colunasEscondidas);
    $listagem->setCabecalho($camposDaTabela);
    $listagem->setQuery($sql);
    $listagem->render();
}

function getColunasEscondidas()
{
    return array('reaprazo', 'dmdid', 'responsavel', 'cods', 'dmeid');
}

function getCamposDaTabela()
{
    return array(
        'cont' => '',
        'autor' => 'Autor',
        'dmdprocesso' => 'Nº Processo',
        'objnome' => 'Objeto',
        'acjnome' => 'Tipo Ação',
        'dmduf' => 'UF',
        'dpanome' => 'Réus',
        'prazo_resp_prof' => 'Data PROFE',
        'dias_em_atraso' => 'Dias em Atraso Total',
        'esddsc' => 'Situação',
//        'dmdcumprimento' => 'Cumprimento da Área técnica',
        'dmdstatusdecisao' => 'Status Decisão',
        'dmddtentradaprofe' => 'Data Entrada PROFE',
        'codigo' => 'Código',
    );
}

function getCamposDaTabelaEntrega()
{
    return array('dmedsc' => 'Descrição', 'dmedtinclusao' => 'Dt. Inclusão', 'dmetipo' => 'Tipo', 'dmerelocorrencia' => 'Relatório de Ocorrência', 'esddsc_entrega' => 'Estado', 'data_estado_entrega' => 'Data Alteração Estado');
}

function getSqlListagemProfe()
{
    $perfis = pegaPerfilGeral($_SESSION['usucpf']);

    $esd_nucleo_juridico = ESD_DEMANDA_NUCLEO_JURIDICO;
    $esd_em_cadastramento = ESD_DEMANDA_EM_CADASTRAMENTO;
    $esd_profe = ESD_DEMANDA_PROFE;

    $sql = "
	WITH responsavelarea AS (SELECT rea.dmdid,
                rea.usucpf,
                rea.reaprazo,
                rea.esdid
           FROM demandasfies.responsavelarea rea
           WHERE rea.reastatus = 'A'),
     documento AS (SELECT doc.*,
           CASE WHEN {$esd_profe} = doc.esdid THEN {$esd_em_cadastramento}
                   ELSE doc.docid
                    END AS esdid1
            FROM workflow.documento doc
            WHERE EXISTS (SELECT 1
                         FROM demandasfies.demanda d
                         WHERE d.docid = doc.docid)),
     autor_reu AS (SELECT array_to_string(array_agg(dpa.dpanome || ' - ' || COALESCE(public.formata_cpf_cnpj(dpacpf), '') || COALESCE(public.formata_cpf_cnpj(dpacnpj), '')), ', ') AS dpanome,
                          dpa.dmdid,
                          dpa.dpatipo
                     FROM demandasfies.demandapartesacao dpa
                     WHERE dpa.dpastatus = 'A'
                       AND EXISTS (SELECT 1
                                     FROM demandasfies.demanda d
                                     WHERE dpa.dmdid = d.dmdid)
                     GROUP BY dpa.dmdid,
                              dpa.dpatipo),
     nome_objeto AS (SELECT dcl.dmdid,
                            array_to_string(array_agg(clanome), ', ') AS clanome
                       FROM demandasfies.demandaclassificacao dcl
                         INNER JOIN demandasfies.classificacao cl ON (cl.claid = dcl.claid)
                       WHERE dcl.dmcstatus = 'A'
                         AND cl.clastatus = 'A'
                       GROUP  BY dcl.dmdid)

            SELECT DISTINCT (d.dmdid, de.dmeid) as cods ,
                1 as cont,
                ---- ENTREGA
                de.dmeid,
                de.dmedsc,
                to_char(de.dmedtinclusao, 'DD/MM/YYYY HH24:mi:ss') as dmedtinclusao,
                ed.esddsc,
                CASE WHEN de.dmetipo = 'ET' THEN 'Entrega Total'
                     WHEN de.dmetipo = 'EP' THEN 'Entrega Parcial'
                  END as dmetipo,
                de.usucpfinclusao,
               CASE WHEN de.dmerelocorrencia = 'f' THEN 'NÃO'
                               WHEN de.dmerelocorrencia = 't' THEN 'SIM'
               END as dmerelocorrencia,
               est_entrega.esddsc as esddsc_entrega,
               to_char(hd_entrega.htddata , 'DD/MM/YYYY HH24:mi:ss') as data_estado_entrega,

               ---- DEMANDA
               to_char(d.dmddtentradaprofe , 'DD/MM/YYYY') as dmddtentradaprofe,
               d.dmdid as codigo,
               de.dmeid,
                nome_objeto.clanome AS objnome,
                autor.dpanome AS autor,
                (SELECT usunome from seguranca.usuario usu where usu.suscod != 'B' and  usu.usucpf =  rea.usucpf) AS responsavel,

                               dmdprocesso,
                               ed.esddsc,
                               acjnome,
                               CASE
                                               WHEN EXTRACT(DAYS FROM(NOW() - reaNucleoJuridico.reaprazo ))::integer > 0
                                               THEN EXTRACT(DAYS FROM(NOW() - reaNucleoJuridico.reaprazo ))::integer  || ' dias'
                                               ELSE ''
                               end as dias_em_atraso,
                               TO_CHAR(reaProfe.reaprazo, 'DD/MM/YYYY') as prazo_resp_prof,
                               d.dmduf,
                               CASE
                                               WHEN d.dmdstatusdecisao = 'DF' THEN 'Deferida'
                                               WHEN d.dmdstatusdecisao = 'ID' THEN 'Indeferida'
                                               WHEN d.dmdstatusdecisao = 'PD' THEN 'Parcialmente Deferida'
                               end as dmdstatusdecisao,
                              /* CASE
                                               WHEN d.dmdcumprimento = 'DC' THEN 'Decisão Cumprida'
                                               WHEN d.dmdcumprimento = 'DCP' THEN 'Decisão Cumprida Parcialmente'
                                               WHEN d.dmdcumprimento = 'DNC' THEN 'Decisão Não Cumprida'
                               end as dmdcumprimento,*/

                               rea.reaprazo,
                               reu.dpanome as dpanome
                FROM demandasfies.demanda d
                INNER JOIN documento doc ON doc.docid = d.docid
                INNER JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
                LEFT JOIN workflow.historicodocumento hd ON hd.hstid = doc.hstid
                LEFT JOIN responsavelarea rea ON (rea.dmdid = d.dmdid AND rea.esdid = doc.esdid1)
                LEFT  JOIN demandasfies.responsavelarea reaNucleoJuridico ON (reaNucleoJuridico.dmdid, reaNucleoJuridico.esdid, reaNucleoJuridico.reastatus) = (d.dmdid, {$esd_nucleo_juridico}, 'A')
                LEFT  JOIN demandasfies.responsavelarea reaProfe ON (reaProfe.dmdid, reaProfe.esdid, reaProfe.reastatus) = (d.dmdid, {$esd_em_cadastramento}, 'A')
                LEFT  JOIN demandasfies.acaojudicial acj ON acj.acjid = d.acjid
                LEFT  JOIN demandasfies.demandaentrega de ON de.dmdid = d.dmdid

                LEFT JOIN workflow.documento doc_entrega ON doc_entrega.docid = de.docid
                LEFT JOIN workflow.estadodocumento est_entrega on est_entrega.esdid =doc_entrega.esdid
                LEFT JOIN workflow.historicodocumento hd_entrega ON hd_entrega.docid = doc_entrega.docid AND hd_entrega.hstid = doc_entrega.hstid

                LEFT JOIN autor_reu autor ON (autor.dmdid = d.dmdid AND autor.dpatipo = 'A')
                LEFT JOIN autor_reu reu ON (reu.dmdid = d.dmdid AND reu.dpatipo = 'R')
                LEFT JOIN nome_objeto ON (nome_objeto.dmdid = d.dmdid)
              ";
    return $sql;
}

function getAdvogados()
{
    $sisid = SIS_DEMANDASFIES;
    $pfl_adv = PFL_ADVOGADO;
    $sql = "
			SELECT usu.usucpf as codigo, usu.usunome as descricao FROM seguranca.usuario AS usu
				INNER JOIN seguranca.usuario_sistema AS ususis ON ususis.usucpf = usu.usucpf
				INNER JOIN seguranca.perfilusuario AS perf ON perf.usucpf = usu.usucpf
				WHERE perf.pflcod = {$pfl_adv} AND ususis.sisid = {$sisid}
			";
    return $sql;
}

function organizarDemandasEntregas($dados)
{
    $entrega = $demanda = array();
    foreach ($dados as $key => $valor) {
        $demanda[$valor['codigo']] = $valor;
        if (!empty($valor['dmeid'])) {
            $entrega[$valor['codigo']][$valor['dmeid']] = $valor;
        }
    }
    foreach ($entrega as $dmdid => $entregas) {
        $demanda[$dmdid]['entrega'] = $entrega[$dmdid];
    }
    return $demanda;
}

function getSqlPesquisa(Demanda $modelDemanda, DemandaClassificacao $modelDemandaClassificacao, $comfitro = true)
{
    $sql = getSqlListagemProfe();

    $claids = $_POST['claid'];

    if ($_POST ['action'] == 'pesquisar' && $comfitro) {

        $modelDemanda->popularDadosObjeto();
        $esdid_entrega = $_POST['esdid_entrega'];
        $where = $join = array();

        $pendecia = (int)$_POST['pendecia'];
        $dt_inicio = formata_data_sql($_POST['dt_inicio']);
        $dt_fim = formata_data_sql($_POST['dt_fim']);

        if (!empty($pendecia)) {
            $where [] = " est_entrega.esdid = {$pendecia} ";
        }
        if (!empty($dt_inicio)) {
            $where[] = " hd_entrega.htddata >= '{$dt_inicio} 00:00:00' ";
        }
        if (!empty($dt_fim)) {
            $where[] = " hd_entrega.htddata <=  '{$dt_fim} 23:00:00' ";
        }

        if ($modelDemanda->dmdid) {
            $where [] = "d.dmdid = {$modelDemanda->dmdid}";
        }
        if ($modelDemanda->dmdtipo) {
            $where [] = "d.dmdtipo = '{$modelDemanda->dmdtipo}' ";
        }
        if (!empty($claids)) {
            $stringClaids = implode(' , ', $claids);
            $where [] = " 	cl.claid IN ({$stringClaids}) ";
            $where [] = " 	clatipo = 'G' ";
            $join[] = " INNER JOIN demandasfies.demandaclassificacao dcl ON d.dmdid = dcl.dmdid
						INNER JOIN demandasfies.classificacao cl ON cl.claid = dcl.claid ";
        }
        if ($modelDemanda->dmdstatusdecisao) {
            $where [] = "d.dmdstatusdecisao = '{$modelDemanda->dmdstatusdecisao}' ";
        }
        if ($modelDemanda->trbid) {
            $where [] = "d.trbid = {$modelDemanda->trbid}";
        }
        if ($modelDemanda->dmduf) {
            $where [] = "d.dmduf = '{$modelDemanda->dmduf}' ";
        }
        if ($modelDemanda->dmdcumprimento) {
            $where [] = "d.dmdcumprimento = '{$modelDemanda->dmdcumprimento}' ";
        }
        if ($modelDemanda->dmdprazo) {
            $dmdprazo = formata_data_sql($modelDemanda->dmdprazo);
            $where [] = "d.dmdprazo = '{$dmdprazo}'";
        }
        if ($modelDemanda->dmdprocesso) {
            $where [] = "d.dmdprocesso ilike '%{$modelDemanda->dmdprocesso}%' ";
        }
        if ($modelDemanda->dmdliminar) {
            $where [] = "d.dmdliminar = '{$modelDemanda->dmdliminar}' ";
        }
        if ($modelDemanda->dmdmulta) {
            $where [] = "d.dmdmulta = '{$modelDemanda->dmdmulta}' ";
        }
        if (!empty($_REQUEST['esdid'])) {
            $where [] = " doc.esdid = {$_REQUEST['esdid']} ";
        }
        if (!empty($_REQUEST['usucpf'])) {
            $where [] = " usu.usucpf = '{$_REQUEST['usucpf']}' ";
            $join[] = " INNER JOIN seguranca.usuario usu ON usu.usucpf = rea.usucpf  ";
        }

        if (!empty($_POST['area'])) {

            switch ($_POST['area']) {
                case (PFL_PROCURADOR_FEDERAL):
                    $pesquisa = ' doc.esdid in (' . ESD_DEMANDA_EM_CADASTRAMENTO . ') ';
                    break;
                case (PFL_NUCLEO_JURIDICO):
                    $pesquisa = ' doc.esdid in (' . ESD_DEMANDA_NUCLEO_JURIDICO . ', ' . ESD_DEMANDA_ADVOGADO . ') ';
                    break;
                case (PFL_DTI_MEC):
                    $pesquisa = ' doc.esdid in (' . ESD_DEMANDA_DTI_MEC . ', ' . ESD_DEMANDA_EXECUCAO_DTI_MEC . ', ' . ESD_DEMANDA_ANALISTA_DTI_MEC . ') ';
                    break;
                case (PFL_4_NIVEL):
                    $pesquisa = ' doc.esdid in (' . ESD_DEMANDA_4_NIVEL . ') ';
                    break;
                case (PFL_GESTOR_FIES):
                    $pesquisa = ' doc.esdid in (' . ESD_DEMANDA_GESTOR_FIES . ') ';
                    break;
                default:
                    $pesquisa = ' doc.esdid in (' . $_POST['area'] . ') ';
            }
            $where [] = $pesquisa;
        }
        $join = array_filter($join);

        if (!empty ($join)) {
            $join = implode('  ', $join);
            $sql .= $join;
        }

        $sql .= " WHERE d.dmdstatus = 'A' ";

        $where = array_filter($where);
        if (!empty ($where)) {
            $where = implode(' AND ', $where);
            $sql .= ' AND ' . $where;
        }
    } else {
        $sql .= " WHERE d.dmdstatus = 'A' ";
    }
    $sql .= ' ORDER BY autor --LIMIT 300';
    return $sql;
}