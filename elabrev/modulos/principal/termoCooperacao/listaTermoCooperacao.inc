<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
function pesquisaRapida(id) {
    jQuery.expr[':'].contains = function(a, i, m) {
        return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
    };

    $("#"+id).keyup(function(){
        $('table.table tbody tr td').removeClass('marcado');
        $('table.table tbody tr').removeClass('remover');
        stringPesquisa = $("#"+id).val();
        if (stringPesquisa) {
            $('table.listagem tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
            $('table.listagem tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
        } else {
            $("table.listagem tbody tr td").removeClass("marcado");
            $("table.listagem tbody tr").removeClass("remover");
        }
    });
}


$(document).ready(function(){
    pesquisaRapida("textFind");

    $('.novo').click(function(){
        window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=proponente&ungcod='+$('[name=ungcod]').val();
    });

    $('.alterar').live('click',function(){
        arDados = $(this).attr('id').split('_');
        var tcpid = arDados[0];
        var esdid = arDados[1];

        var aba = 'proponente';
        if(esdid == <?php echo EM_EXECUCAO ?>){
            aba = 'cumprimento';
        }else
        if(esdid == <?php echo ALTERAR_TERMO_COOPERACAO ?>){
            aba = 'previsao';
        }
        window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba='+aba+'&tcpid='+tcpid;
    });

    $('.alteracao').live('click',function(){
        var tcpid = $(this).attr('id');
        window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao&tcpid='+tcpid;
    });

    $('.enviarParaDiligencia').click(function(){
        arIds = this.id.split('_');
        docid = arIds[0];
        tcpid = arIds[1];
        if(confirm('Deseja enviar o Termo de Execução Descentralizada nº '+tcpid+'para diligência?')){
            $.ajax({
                url		: 'elabrev.php?modulo=principal/termoCooperacao/listaTermoCooperacao&acao=A',
                type	: 'post',
                data	: 'requisicao=enviarParaDiligencia&docid='+docid,
                success	: function(e){
                    if(e == '1'){
                        alert('Operação realizada com sucesso.');
                    }else{
                        alert('Falha ao enviar o termo para diligência!');
                    }
                    window.reload();
                }
            });
        }
    });

    $('[name=tcpstatusanalise[]]').change(function(){

        arDados = this.id.split('_');

        if(confirm('Deseja realmente mudar o status deste termo?')){

            $.ajax({
                url		: 'elabrev.php?modulo=principal/termoCooperacao/listaTermoCooperacao&acao=A',
                type	: 'post',
                data	: 'requisicao=gravaStatusAnalise&tcpid='+arDados[1]+'&status='+this.value+'&docid='+arDados[2],
                success	: function(e){
                    if(e == '1'){
                        alert('Operação realizada com succeso.');
                        window.location.reload();
                    }else{
                        alert('Falha na operação!');
                    }
                }
            });
        }
    });

    $('#btnLimpar').click(function(){
        document.location = document.location;
    });
});
</script>
<style type="text/css">
.fullTextInputSize {
    width: 100%;
}
.marcado {
    background-color: #C1FFC1 !important;
}
.remover {
    display: none;
}
</style>
<?php

if ($_REQUEST['requisicao'] == 'enviarParaDiligencia') {
	if ($_REQUEST['docid']) {
		include_once APPRAIZ . "includes/workflow.php";
		define('AEDID_ENVIAR_PARA_DILIGENCIA', 1523);
		if (wf_alterarEstado($_REQUEST['docid'],AEDID_ENVIAR_PARA_DILIGENCIA, ' ', array())) {
			echo '1';
			die();
		}
	}
	echo '2';
	die();
}

if ($_REQUEST['requisicao'] == 'gravaStatusAnalise') {
	$sql = "update monitora.termocooperacao set tcpstatusanalise = {$_REQUEST['status']} where tcpid = {$_REQUEST['tcpid']}";
	$db->executar($sql);
	if ($db->commit()) {
		if ($_REQUEST['status'] == 2) {
			if ($_REQUEST['docid']) {
				include_once APPRAIZ . "includes/workflow.php";
				if (wf_alterarEstado($_REQUEST['docid'],AEDID_ARQUIVAR_TERMO, ' ', array())) {
					die('1');
				}
			}
		}
		die('1');
	}
	die('0');
}

require_once '_funcoes_termoCooperacao.php';	
require_once APPRAIZ . 'includes/cabecalho.inc';

if ($_REQUEST['ungcod']) {
	$_SESSION['elabrev']['ungcodlista'] = $_REQUEST['ungcod'];
}

$_SESSION['elabrev']['tcpid'] = null;
$perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

echo '<br/>';

if (strstr($_SESSION['favurl'], 'listaTermoCooperacaoGeral')) {
	montaAbaTermoCooperacaoListaUGs();
} else {
	montaAbaTermoCooperacaoListaTermos();
}

monta_titulo('Lista de Termo de Execução Descentralizada Geral', '');
montaCabecalhoUG();

if ($_POST) {
    extract($_POST);
}

?>

<?php if(!strstr($_SESSION['favurl'], 'listaTermoCooperacaoGeral') && (in_array(UO_EQUIPE_TECNICA, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis))): ?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" >
		<tr>
			<td colspan="3">
				<a href="#" class="novo"> 
					<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Novo Termo de Execução Descentralizada"> Novo Termo de Execução Descentralizada
				</a>
			</td>
		</tr>
	</table>
<?php endif; ?>

<form method="post" name="formulario" id="formulario">
	<input type="hidden" id="pesquisa" name="pesquisa" value="1" />
	<input type="hidden" id="ungcod" name="ungcod" value="<?php echo $_REQUEST['ungcod'] ? $_REQUEST['ungcod'] : '' ?>" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tbody>
			<tr>
				<td class="SubTituloDireita" valign="bottom">Número do Termo</td>
				<td>
					<?php 
						$tcpid = $_POST['tcpid']; 
						echo campo_texto( 'tcpid', 'N', 'S', '', 25, 200, '#######', '' ); 
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="bottom">Número da Unidade Orçamentária</td>
				<td>
					<?php 
						$unicod = $_POST['unicod']; 
						echo campo_texto( 'unicod', 'N', 'S', '', 25, 200, '', '' ); 
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="bottom">Unidade Gestora Proponente</td>
				<td>				
					<?php
						$sql = "SELECT 
									ungcod as codigo, 
									ug.ungcod || ' - ' || ungdsc as descricao
								FROM public.unidadegestora ug
									inner join public.unidade u ON u.unicod = ug.unicod
								WHERE ungstatus='A'   
								ORDER BY 2";					
						$db->monta_combo('ungcodproponente',$sql, 'S', 'Selecione...', '', '', '', 260, 'N', 'ungcodconcedente', '', '', '');
					?>
				</td>
			</tr>
			
			<tr>
				<td class="SubTituloDireita" valign="bottom">Unidade Gestora Concedente</td>
				<td>
					<?php
						$sql = "SELECT	ungcod as codigo,
										ungabrev||' / '||ungdsc as descricao
								FROM public.unidadegestora
								WHERE ungstatus = 'A'
								and ungcod in ('155007', '150004', '150002', '150001','150003','150019','150016','150011','150028','152390','152389','152734','153173','154003','153978','150014')
								ORDER BY 2";
                        if(in_array(PERFIL_DIRETORIA_FNDE, $perfis) || in_array(PERFIL_AREA_TECNICA_FNDE, $perfis)){
//                            $where[] = "unc.ungcod = '" . UG_FNDE ."'";
                            $ugid = UG_FNDE;
                            $habUg = 'N';
                        } else {
                            $ugid = '';
                            $habUg = 'S';
                        }

						$db->monta_combo('ungcodconcedente',$sql, $habUg, 'Selecione...', '', '', '', 260, 'N', 'ungcodconcedente', '', $ugid, '');
					?>
				</td>
			</tr>
			
			<tr>
				<td class="SubTituloDireita" valign="bottom">Situação do termo</td>
				<td>
					<?php 
						$sql =  "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = ".WF_TIPOS_DE_DOCUMENTOS." and esdstatus = 'A' order by esdordem";
						$db ->monta_combo('esdid', $sql, 'S', 'Selecione', '','','','','','esdid');
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" colspan="2" align="center" style="text-align:center"> 
	         		<input type="submit" name="btnPesquisar" id="btnPesquisar" value="Pesquisar" />
	         		<input type="button" name="btnLimpar" id="btnLimpar" value="Limpar" /> 
				</td>
	     	</tr>
		</tbody>
	</table>
</form>

<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tbody>
        <tr>
            <td>
                <label for="textFind">Pesquisa</label>
                <input type="text" name="textFind" class="fullTextInputSize" id="textFind" value="">
            </td>
        </tr>
    </tbody>
</table>
<?php 

if ($_REQUEST['ungcod']) {

    $orPolitica = " or tcp.ungcodpoliticafnde = '{$_REQUEST['ungcod']}'";

    if (!in_array(PERFIL_SUPER_USUARIO, $perfis)) {
        if (verificaPossuiPerfilConcProp()) {
            $where[] = "(unp.ungcod = '{$_REQUEST['ungcod']}' or unc.ungcod = '{$_REQUEST['ungcod']}' {$orPolitica})";
        } elseif(verificaPossuiPerfilConcedente()) {
            $where[] = "( unc.ungcod = '{$_REQUEST['ungcod']}'".$orPolitica." )";
        } elseif(verificaPossuiPerfilProponente()) {
            $where[] = "( unp.ungcod = '{$_REQUEST['ungcod']}'".$orPolitica." )";
        }
    } else {
        $where[] = "(unp.ungcod = '{$_REQUEST['ungcod']}' or unc.ungcod = '{$_REQUEST['ungcod']}' {$orPolitica})";
    }
}
	
$where[] = "tcpstatus = 'A'";

if ($_POST['tcpid'])
    $where[] = "tcp.tcpid = {$tcpid}";

if ($_POST['unicod'])
    $where[] = "(unp.unicod = '{$unicod}' or unc.unicod = '{$unicod}')";

if ($_POST['ungcodconcedente'])
    $where[] = "unc.ungcod = '{$ungcodconcedente}'";

if ($_POST['ungcodproponente'])
    $where[] = "unp.ungcod = '{$ungcodproponente}'";

if ($_POST['esdid'])
    $where[] = "doc.esdid = {$esdid}";

// Se não for super usuário seta as responsabilidades
if (!in_array(PERFIL_SUPER_USUARIO, $perfis)) {

    if (verificaPossuiPerfilConcProp()) {

        if (isset($_SESSION['elabrev']['prop_conc']['ungcod']) && !empty($_SESSION['elabrev']['prop_conc']['ungcod'])) {
            $where[] = is_array($_SESSION['elabrev']['prop_conc']['ungcod']) ?
                "(unp.ungcod in ('".implode("','", $_SESSION['elabrev']['prop_conc']['ungcod'])."') OR unc.ungcod in ('".implode("','", $_SESSION['elabrev']['prop_conc']['ungcod'])."') {$orPolitica})" :
                "(unp.ungcod = '{$_SESSION['elabrev']['prop_conc']['ungcod']}' OR unc.ungcod = '{$_SESSION['elabrev']['prop_conc']['ungcod']}' {$orPolitica})";
        } else {
            if (isset($_SESSION['elabrev']['proponente']['ungcod']) && !empty($_SESSION['elabrev']['proponente']['ungcod'])) {
                $whereTemp[] = is_array($_SESSION['elabrev']['proponente']['ungcod']) ?
                    "unp.ungcod IN ('".implode("','", $_SESSION['elabrev']['proponente']['ungcod'])."')" :
                    "unp.ungcod = '{$_SESSION['elabrev']['proponente']['ungcod']}'";
            }

            if (isset($_SESSION['elabrev']['concedente']['ungcod']) && !empty($_SESSION['elabrev']['concedente']['ungcod'])) {
                $whereTemp[] = is_array($_SESSION['elabrev']['concedente']['ungcod']) ?
                    "unc.ungcod IN ('".implode("','", $_SESSION['elabrev']['concedente']['ungcod'])."')" :
                    "unc.ungcod = '{$_SESSION['elabrev']['concedente']['ungcod']}'";
            }
        }

        if ($whereTemp)
            $where[] = " (".implode(' OR ', $whereTemp)." {$orPolitica}) ";

    } else

    // Verifica se tem perfil de Proponente
    //Gestor Orçamentário do Proponente - 852
    //Representante Legal do Proponente - 864
    if (in_array(PERFIL_PROREITOR_ADM, $perfis) || in_array(PERFIL_REITOR, $perfis)) {
        if (isset($_SESSION['elabrev']['proponente']['ungcod']) && !empty($_SESSION['elabrev']['proponente']['ungcod'])) {
            $where[] = is_array($_SESSION['elabrev']['proponente']['ungcod']) ?
                "unp.ungcod in ('".implode("','", $_SESSION['elabrev']['proponente']['ungcod'])."')" :
                "unp.ungcod = '{$_SESSION['elabrev']['proponente']['ungcod']}'";
        } else {
            $where[] = "unp.ungcod IS NULL";
        }
    } else

    // Verifica se tem perfil de UO / Equipe Tecnica
    if (in_array(UO_EQUIPE_TECNICA, $perfis)) {
        if (isset($_SESSION['elabrev']['prop_conc']['ungcod']) && !empty($_SESSION['elabrev']['prop_conc']['ungcod'])) {
            $where[] = is_array($_SESSION['elabrev']['prop_conc']['ungcod']) ?
                "( unp.ungcod in ('".implode("','", $_SESSION['elabrev']['prop_conc']['ungcod'])."') or unc.ungcod in ('".implode("','", $_SESSION['elabrev']['prop_conc']['ungcod'])."') or tcp.ungcodpoliticafnde in ('".implode("','", $_SESSION['elabrev']['prop_conc']['ungcod'])."') )" :
                "( unp.ungcod = '{$_SESSION['elabrev']['prop_conc']['ungcod']}' or unc.ungcod = '{$_SESSION['elabrev']['prop_conc']['ungcod']}' or tcp.ungcodpoliticafnde = '{$_SESSION['elabrev']['prop_conc']['ungcod']}' )";
        } else {
            $where[] = "unp.ungcod IS NULL";
            $where[] = "unc.ungcod IS NULL";
        }
    } else

    // Verifica se tem perfil de Concedente para Unidade Gestora e Unidade Gestora Direta
    if (in_array(UO_COORDENADOR_EQUIPE_TECNICA, $perfis) ||
        in_array(PERFIL_SECRETARIA, $perfis) ||
        in_array(PERFIL_SECRETARIO, $perfis))
    {
        if (isset($_SESSION['elabrev']['concedente']['ungcod']) && !empty($_SESSION['elabrev']['concedente']['ungcod'])) {

            $orPolitica = '';
            if (in_array(PERFIL_SECRETARIA, $perfis) || in_array(PERFIL_SECRETARIO, $perfis))
                $orPolitica = is_array($_SESSION['elabrev']['concedente']['ungcod']) ?
                    " or tcp.ungcodpoliticafnde in ('".implode("','", $_SESSION['elabrev']['concedente']['ungcod'])."')" :
                    " or tcp.ungcodpoliticafnde = '{$_SESSION['elabrev']['concedente']['ungcod']}'";

            $where[] = is_array($_SESSION['elabrev']['concedente']['ungcod']) ?
                "( unc.ungcod in ('".implode("','", $_SESSION['elabrev']['concedente']['ungcod'])."')".$orPolitica." )" :
                "( unc.ungcod = '{$_SESSION['elabrev']['concedente']['ungcod']}'".$orPolitica." )";
        } else {
            $where[] = "unc.ungcod IS NULL";
        }
    } else

    // Verifica se tem perfil de Concedente para CGSO e Gestor Orçamentario do Concedente, Unidade Gestora
    if (in_array(PERFIL_SUBSECRETARIO, $perfis) || in_array(PERFIL_CGSO, $perfis)) {
        if (isset($_SESSION['elabrev']['concedente']['ungcod']) && !empty($_SESSION['elabrev']['concedente']['ungcod'])) {
            if (is_array($_SESSION['elabrev']['concedente']['ungcod'])) {
                if (in_array(UG_CGSO, $_SESSION['elabrev']['concedente']['ungcod'])) {
                    $where[] = "unc.ungcod NOT IN ('153978','154003','153173')";
                }
            } else if($_SESSION['elabrev']['concedente']['ungcod'] == UG_CGSO) {
                $where[] = "unc.ungcod NOT IN ('153978','154003','153173')";
            } else {
                $where[] = is_array($_SESSION['elabrev']['concedente']['ungcod']) ?
                    "unc.ungcod IN ('".implode("','", $_SESSION['elabrev']['concedente']['ungcod'])."')" :
                    "unc.ungcod = '{$_SESSION['elabrev']['concedente']['ungcod']}'";
            }
        }
    }else

    // Verifica se tem perfil de Concedente para Diretoria
    if (in_array(PERFIL_DIRETORIA, $perfis)) {
        if (isset($_SESSION['elabrev']['concedente']['dircod']) && !empty($_SESSION['elabrev']['concedente']['dircod'])) {
            $inner[] = "INNER JOIN elabrev.diretoria dir ON (dir.ungcod = tcp.ungcodconcedente or dir.ungcod = tcp.ungcodpoliticafnde)";
            $where[] = is_array($_SESSION['elabrev']['concedente']['dircod']) ?
                "dir.dircod in ('".implode("','", $_SESSION['elabrev']['concedente']['dircod'])."')" :
                "dir.dircod = '{$_SESSION['elabrev']['concedente']['dircod']}'";
        } else {
            $where[] = "unc.ungcod IS NULL";
        }
    } else

    // Verifica se tem perfil de Concedente para Coordenador
    if (in_array(PERFIL_COORDENADOR_SEC, $perfis)) {
        if (isset($_SESSION['elabrev']['concedente']['cooid']) && !empty($_SESSION['elabrev']['concedente']['cooid'])) {
            $where[] = is_array($_SESSION['elabrev']['concedente']['cooid']) ?
                "tcp.cooid in ('".implode("','", $_SESSION['elabrev']['concedente']['cooid'])."')" :
                "tcp.cooid = '{$_SESSION['elabrev']['concedente']['cooid']}'";
        } else {
            $where[] = "unc.ungcod IS NULL";
        }
    } else

    // Verifica se tem perfil de Concedente para Unidade
    if (in_array(UO_CONSULTA_ORCAMENTO, $perfis) ||
            in_array(AUDITOR_INTERNO, $perfis) ||
            in_array(PERFIL_DIRETOR_ADMIM, $perfis))
    {
        if (isset($_SESSION['elabrev']['concedente']['unicod']) && !empty($_SESSION['elabrev']['concedente']['unicod'])) {
            $where[] = is_array($_SESSION['elabrev']['concedente']['unicod']) ?
                "unc.unicod in ('".implode("','", $_SESSION['elabrev']['concedente']['unicod'])."')" :
                "unc.unicod = '{$_SESSION['elabrev']['concedente']['unicod']}'";
        } else {
            $where[] = "unc.unicod IS NULL";
        }
    }

    if (in_array(PERFIL_DIRETORIA_FNDE, $perfis) || in_array(PERFIL_AREA_TECNICA_FNDE, $perfis)) {
        $where[] = "unc.ungcod = '" . UG_FNDE ."'";
    }
}

$where[] = "tcp.tcpid in (select distinct tc.tcpid from monitora.termocooperacao tc
            left join monitora.previsaoorcamentaria po on tc.tcpid = po.tcpid
            where tcpstatus = 'A')";

$sqlCountSolAteracao = " (select count(*) from workflow.historicodocumento hst where hst.aedid = ".WF_ACAO_SOL_ALTERACAO." and hst.docid = tcp.docid) ";

$sql = "
        SELECT
            foo.acao,
            foo.decricao,
            foo.unidadegestorap,
            foo.unidadegestorac,
            coalesce(foo.tcpdscobjetoidentificacao, ' - ') as titulo_obj_despesa,
            foo.esddsc,
            foo.coodsc
        FROM (
            SELECT DISTINCT
                    '<img border=\"0\" id=\"' || tcpid || '_' || esd.esdid || '\" class=\"alterar\" title=\"Editar\" style=\"cursor: pointer;\" src=\"/imagens/alterar.gif\">' as acao,
                    'Termo: '|| tcpid || case when {$sqlCountSolAteracao} > 0 then '.' || {$sqlCountSolAteracao}::varchar else '' end as decricao,
                    unp.ungcod || ' / ' || unp.ungdsc || ' - ' || unp.ungabrev as unidadegestorap,
                    unc.ungcod || ' / ' || unc.ungdsc || ' - ' || unc.ungabrev as unidadegestorac,
                    tcp.tcpdscobjetoidentificacao,
                    ( SELECT us.usunome FROM workflow.historicodocumento hd inner join seguranca.usuario us on us.usucpf = hd.usucpf where hd.aedid=1597 and hd.docid = tcp.docid order by hstid desc limit 1 ) as prop,
                    ( SELECT us.usunome FROM workflow.historicodocumento hd inner join seguranca.usuario us on us.usucpf = hd.usucpf where hd.aedid in (1612, 2442) and hd.docid = tcp.docid order by hstid desc limit 1 ) as conc,
                    ( SELECT us.usunome FROM workflow.historicodocumento hd inner join seguranca.usuario us on us.usucpf = hd.usucpf where hd.aedid=1620 and hd.docid = tcp.docid order by hstid desc limit 1 ) as altprop,
                    esd.esddsc as esddsc,
                    coalesce(cdn.coodsc, '-') as coodsc

            FROM monitora.termocooperacao tcp

            ".(is_array($inner) ? " ".implode(" ", $inner) : '')."
            LEFT JOIN elabrev.coordenacao cdn                ON cdn.cooid = tcp.cooid
            LEFT JOIN public.unidadegestora unp              ON unp.ungcod = tcp.ungcodproponente
            LEFT JOIN public.unidadegestora unc              ON unc.ungcod = tcp.ungcodconcedente
            LEFT JOIN elabrev.representantelegal rpp         ON rpp.ug = tcp.ungcodproponente
            LEFT JOIN elabrev.representantelegal rpc         ON rpc.ug = tcp.ungcodconcedente
            LEFT JOIN workflow.documento doc                 ON doc.docid = tcp.docid
            LEFT JOIN workflow.estadodocumento esd           ON esd.esdid = doc.esdid

            ".(is_array($where) ? "WHERE ".implode(" AND ", $where) : '')."
            ORDER BY decricao

        ) foo";

$cabecalho = array("Ação", "Termo", "Unidade Gestora <br/>Proponente", "Unidade Gestora <br/>Concedente", "Título / Objeto da despesa", "Situação Documento", "Coordenação");
$alinhamento = array('center');
$tamanho = array(null, null, null, null, null, 250);

$param['ordena'] = false;
$db->monta_lista($sql,$cabecalho,50,5,'N','center','', '', $tamanho, $alinhamento, null, $param);
//ver($sql);
?>