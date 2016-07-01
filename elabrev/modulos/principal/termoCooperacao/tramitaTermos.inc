<?php 

include_once "_funcoes.php";
require_once '_funcoes_termoCooperacao.php';

if (isset($_GET['geraDoc']) && ($_GET['geraDoc'] == 'true')) {
    if (count($_SESSION['elabrev']['extratodou'])) {

        gerarExtratoLote($_SESSION['elabrev']['extratodou']);
        unset($_SESSION['elabrev']['extratodou']);

    } else {
        echo "<script>
              alert('Nenhum extrato para ser gerado!');
              window.location.href = 'elabrev.php?modulo=principal/termoCooperacao/tramitaTermos&acao=A';
          </script>";
    }
}

function verificarComentario($dados) {
	global $db;
	
	ob_clean();
	
	$esdsncomentario = $db->pegaUm("SELECT esdsncomentario FROM workflow.acaoestadodoc WHERE aedid='".$dados['aedid']."'");
	
	if ($esdsncomentario == 't') {
		echo 'TRUE';
	} else {
		echo 'FALSE';
	}
}

function tramitar($dados) {
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	$sql = "select grpid from monitora.tramitalote_grupo where tpgid = ".LOTE_TIPO_DESCENTRALIZACAO." and usucpf = '".str_pad($_SESSION['usucpf'],11,0,STR_PAD_LEFT)."'";
	$grpid = $db->pegaUm($sql);
	
	if(!$grpid){	
		$sql = "INSERT INTO monitora.tramitalote_grupo(tpgid, usucpf) VALUES(".LOTE_TIPO_DESCENTRALIZACAO.", '".str_pad($_SESSION['usucpf'],11,0,STR_PAD_LEFT)."') RETURNING grpid";		
		$grpid = $db->pegaUm($sql);
	}

	$sqlItens = '';
    $_SESSION['elabrev']['extratodou'] = array();
	foreach ($dados['docid'] as $prcid => $docid) {
	
		$sql = "select tcpid from monitora.termocooperacao where docid = {$docid}";
		$_SESSION['elabrev']['tcpid'] = $db->pegaUm($sql);
        array_push($_SESSION['elabrev']['extratodou'], $_SESSION['elabrev']['tcpid']);

		$test = wf_alterarEstado($docid, $dados['aedid'], $dados['comentario'], array('docid' => $docid, 'prcid' => $prcid, 'emlote' => '1', "cooid" => $dados['cooid'], "advid" => $dados['advid']));

		if ($test) {
			$sql = "SELECT
						max(hstid) as hstid
					FROM
						workflow.historicodocumento
					WHERE
						docid = $docid";

			$hstid = $db->pegaUm($sql);

			$sqlItens .= "INSERT INTO monitora.tramitalote_itensgrupo(grpid, hstid) VALUES($grpid, $hstid);";
		}
	}

	unset($_SESSION['elabrev']['tcpid']);
	if( $sqlItens != '' ){
		$db->executar($sqlItens);
		$db->commit();
		$htm = "<script>
					var janela 	= window.open( 'elabrev.php?modulo=principal/termoCooperacao/guiaTramitacao&acao=A&tipo=xls&grpid=$grpid', 'guia', 'width=1300,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
					janela.focus();
				</script>";
		echo $htm;
		
		$db->sucesso("principal/termoCooperacao/tramitaTermos", '');
	}else{
		$sql = "UPDATE monitora.tramitalote_grupo SET
					grpstatus = 'I'
				WHERE
					grpid = $grpid";
		$db->executar();
		$db->commit();
		echo "<script>
				alert('Nenhum termo pôde ser tramitado.');
				window.location.href = 'elabrev.php?modulo=principal/termoCooperacao/tramitaTermos&acao=A';
			  </script>";
	}
}

/**
 * Gerar extrato DOU para termo que são enviado para execução na modalidade tramitação em lote
 * @param array $tcpid
 * @return bool
 */
function gerarExtratoLote(array $arrTcps = array()) {
    include_once APPRAIZ . 'elabrev/classes/modelo/HtmlToDocx.class.inc';
    require_once APPRAIZ . 'elabrev/classes/modelo/ExtratoDOU.class.inc';

    $extrato = new ExtratoDOU();
    $html = $extrato->getLote($arrTcps);
    if ($html) {
        unset($extrato);
        $doc = new HtmlToDocx();
        $doc->setHtml($html)
            ->setNomeDocumento('Extrato_DOU_'.date('d-m-Y--H:i:s', time()))
            ->getDownload();
    }
}

function montaAcoes($dados){
	
	global $db;
	
	$sql = "select true from elabrev.usuarioresponsabilidade where usucpf = '{$_SESSION['usucpf']}' and ungcod = '153173' and rpustatus = 'A'";
	$rs = $db->pegaUm($sql);
	
	$where = '';
	if(in_array($dados['esdid'], array(642,637,635)) && !$db->testa_superuser()){
		if($rs == 't'){			
			$where .= " and aedcondicao ilike '%verificaConcedenteFnde(%' ";
		}else{
			$where .= " and aedcondicao not ilike '%verificaConcedenteFnde(%' ";
		}
	}
	
	$sql = "SELECT DISTINCT
				aed.aedid as codigo,
				aed.aeddscrealizar as descricao  
			FROM 	
				workflow.acaoestadodoc aed 
			INNER JOIN workflow.estadodocumentoperfil 	esp ON esp.aedid 	= aed.aedid
			INNER JOIN seguranca.perfilusuario 			pus ON pus.pflcod 	= esp.pflcod
			WHERE 
				aedstatus = 'A' 
				AND aed.esdidorigem = ".$dados['esdid']." 
				--AND pus.usucpf = '{$_SESSION['usucpf']}'
				AND aed.aedvisivel = 't'
				{$where}
			ORDER  BY 
				2";
	
	$db->monta_combo('aedid',$sql,'S','Selecione...','','','','','S', 'aedid');
}

function lista($dados = null) {
	
	global $db;

    //ver($_POST);
	
	if ($_POST['aedid'] == '' || $_POST['esdid'] == '') {
		return false;
	}
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	$perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
	
	if($_POST) extract($_POST);
	
	if ($_POST['tcpid'])
		$where[] = "tcpid = {$tcpid}";
	
	if ($_POST['unicod'])
		$where[] = "ung1.unicod = {$unicod}";
	
	if ($_POST['esdid'])
		$where[] = "doc.esdid = {$esdid}";
// 	ver($where,d);
	// Se não for super usuário seta as responsabilidades
	if (!in_array(PERFIL_SUPER_USUARIO, $perfis)) {
	
		if (verificaPossuiPerfilConcProp()) {

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

			if ($whereTemp)
				$where[] = " (".implode(' OR ', $whereTemp).") ";
				
		} else
			
		// Verifica se tem perfil de Proponente
		if (in_array(PERFIL_PROREITOR_ADM, $perfis) || in_array(PERFIL_REITOR, $perfis)) {
			if(isset($_SESSION['elabrev']['proponente']['ungcod']) && !empty($_SESSION['elabrev']['proponente']['ungcod'])){
				$where[] = is_array($_SESSION['elabrev']['proponente']['ungcod']) ?
                    "unp.ungcod in ('".implode("','", $_SESSION['elabrev']['proponente']['ungcod'])."')" :
                    "unp.ungcod = '{$_SESSION['elabrev']['proponente']['ungcod']}'";
			}else{
				$where[] = "unp.ungcod IS NULL";
			}
		}else
	
		// Verifica se tem perfil de UO / Equipe Tecnica
		if (in_array(UO_EQUIPE_TECNICA, $perfis)) {
			if (isset($_SESSION['elabrev']['ungcod']) && !empty($_SESSION['elabrev']['ungcod'])) {
				$where[] = is_array($_SESSION['elabrev']['ungcod']) ?
                    "( unp.ungcod in ('".implode("','", $_SESSION['elabrev']['ungcod'])."') or unc.ungcod in ('".implode("','", $_SESSION['elabrev']['ungcod'])."') )" :
                    "( unp.ungcod = '{$_SESSION['elabrev']['ungcod']}' or unc.ungcod = '{$_SESSION['elabrev']['ungcod']}' )";
			} else {
				$where[] = "unp.ungcod IS NULL";
				$where[] = "unc.ungcod IS NULL";
			}
		} else
	
		// Verifica se tem perfil de Concedente para Unidade Gestora e Unidade Gestora Direta
		if (in_array(UO_COORDENADOR_EQUIPE_TECNICA, $perfis)
            || in_array(PERFIL_SECRETARIA, $perfis)
            || in_array(PERFIL_SECRETARIO, $perfis)) {
			if (isset($_SESSION['elabrev']['concedente']['ungcod']) && !empty($_SESSION['elabrev']['concedente']['ungcod'])) {
				$where[] = is_array($_SESSION['elabrev']['concedente']['ungcod']) ?
                    "unc.ungcod in ('".implode("','", $_SESSION['elabrev']['concedente']['ungcod'])."')" :
                    "unc.ungcod = '{$_SESSION['elabrev']['concedente']['ungcod']}'";
			} else {
				$where[] = "unc.ungcod IS NULL";
			}
		} else

		// Verifica se tem perfil de Concedente para CGSO e Gestor Orçamentario do Concedente, Unidade Gestora
		if (in_array(PERFIL_SUBSECRETARIO, $perfis) || in_array(PERFIL_CGSO, $perfis)) {
			if (isset($_SESSION['elabrev']['concedente']['ungcod']) && !empty($_SESSION['elabrev']['concedente']['ungcod'])) {
				if (is_array($_SESSION['elabrev']['concedente']['ungcod'])) {
					if(in_array(UG_CGSO, $_SESSION['elabrev']['concedente']['ungcod'])){
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
		} else
	
		// Verifica se tem perfil de Concedente para Diretoria
		if(in_array(PERFIL_DIRETORIA, $perfis)){
			if(isset($_SESSION['elabrev']['concedente']['dircod']) && !empty($_SESSION['elabrev']['concedente']['dircod'])){
				$inner[] = "INNER JOIN elabrev.diretoria dir ON dir.ungcod = tcp.ungcodconcedente";
				$where[] = is_array($_SESSION['elabrev']['concedente']['dircod']) ?
                    "dir.dircod in ('".implode("','", $_SESSION['elabrev']['concedente']['dircod'])."')" :
                    "dir.dircod = '{$_SESSION['elabrev']['concedente']['dircod']}'";
			}else{
				$where[] = "unc.ungcod IS NULL";
			}
		}else
	
		// Verifica se tem perfil de Concedente para Coordenador
		if(in_array(PERFIL_COORDENADOR_SEC, $perfis)){
			if(isset($_SESSION['elabrev']['concedente']['cooid']) && !empty($_SESSION['elabrev']['concedente']['cooid'])){
				$whereCoo = is_array($_SESSION['elabrev']['concedente']['cooid']) ?
                    "coo.cooid in ('".implode("','", $_SESSION['elabrev']['concedente']['cooid'])."')" :
                    "coo.cooid = '{$_SESSION['elabrev']['concedente']['cooid']}'";

				$inner[] = "INNER JOIN elabrev.coordenacao coo ON coo.ungcodconcedente = tcp.ungcodconcedente AND ".$whereCoo;
			}else{
				$where[] = "unc.ungcod IS NULL";
			}
		}else
		
		// Verifica se tem perfil de Concedente para Unidade
		if(in_array(UO_CONSULTA_ORCAMENTO, $perfis) ||
				in_array(AUDITOR_INTERNO, $perfis) ||
				in_array(PERFIL_DIRETOR_ADMIM, $perfis)) {
			if(isset($_SESSION['elabrev']['concedente']['unicod']) && !empty($_SESSION['elabrev']['concedente']['unicod'])){
				$where[] = is_array($_SESSION['elabrev']['concedente']['unicod']) ?
                    "unc.unicod in ('".implode("','", $_SESSION['elabrev']['concedente']['unicod'])."')" :
                    "unc.unicod = '{$_SESSION['elabrev']['concedente']['unicod']}'";
			}else{
				$where[] = "unc.unicod IS NULL";
			}
		}
	}
	
	$where[] = "tcp.tcpid in (select distinct tc.tcpid from monitora.termocooperacao tc
				left join monitora.previsaoorcamentaria po on tc.tcpid = po.tcpid
				where (po.proanoreferencia >= {$_SESSION['exercicio']} or po.proanoreferencia is null) and tcpstatus = 'A')";
	
	$sql = "SELECT 
				'<center>
				<input type=\"checkbox\" name=\"docid[]\" class=\"doc\" value=\"'|| doc.docid ||'\" />
				<img src=\"../imagens/consultar.gif\" alt=\"Visualizar termo\" title=\"Visualizar termo\" border=\"0\" style=\"cursor:pointer\" id=\"' || tcp.tcpid ||' \" class=\"visualizarTermo\" />
				</center>' as acao,			
				'Termo: '||tcpid as decricao,
				unp.ungcod || ' / ' || unp.ungdsc || ' - ' || unp.ungabrev as unidadegestorap,
				unc.ungcod || ' / ' || unc.ungdsc || ' - ' || unc.ungabrev as unidadegestorac,
				esd.esddsc as esddsc,
				crd.coodsc,
				doc.docid as docid,
				tcp.tcpid
			FROM monitora.termocooperacao tcp	
			".(is_array($inner) ? " ".implode(" ", $inner) : '')."
			LEFT JOIN elabrev.coordenacao crd 
				ON crd.cooid = tcp.cooid
			LEFT JOIN public.unidadegestora unp 
				ON unp.ungcod = tcp.ungcodproponente			
			LEFT JOIN public.unidadegestora unc 
				ON unc.ungcod = tcp.ungcodconcedente
			LEFT JOIN workflow.documento doc  
				ON doc.docid = tcp.docid
			LEFT JOIN workflow.estadodocumento esd  
				ON esd.esdid = doc.esdid		
			".(is_array($where) ? "WHERE ".implode(" AND ", $where) : '')."
			ORDER BY tcp.tcpid";
// 	ver(simec_htmlentities($sql), d);		
	$checkTodos = '<center><input type="checkbox" class="todos" value=""/></center>';
	$arCabecalho = array($checkTodos, "Termo", "Unidade Gestora <br/>Proponente", "Unidade Gestora <br/>Concedente", "Situação Documento", "Coordenação");
	
	unset($_REQUEST['req']);
	unset($_POST['req']);
	if( !($dados = $db->carregar($sql)) ){
		
// 		echo "Ocorreu um erro inesperado.";
		
		 echo '<table border="0" bgcolor="#f5f5f5" align="center" cellspacing="1" cellpadding="3" class="tabela">
				<tbody>
					<tr>		
						<td id="tdacao" colspan="2" class="SubTituloCentro">
							Sem registros.
						</td>
					</tr>
				</tbody>
		 	   </table>';
		
	} 
	
	$linhas = Array();
	$count = 0;
	if ($_POST['aedid'] != '' && is_array($dados)) {
        foreach ($dados as $dado) {
            $_SESSION['elabrev']['tcpid'] = (int) $dado['tcpid'];
			if (wf_acaoPossivel2($dado['docid'], $_POST['aedid'], array( 'docid' => $dado['docid'], 'tcpid' => $dado['tcpid']))) {
				unset($dado['docid']);
				unset($dado['tcpid']);
				$linhas[$count] = $dado;
				$count++;
			}
		}
	} else {
		return false;
	}
	
	$table = '<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
				<tr style="background-color: rgb(230,230,230)">';
	foreach ($arCabecalho as $cabecalho) {
		$table .= '<td valign="top" bgcolor="" align="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" 
            style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
            class="title"><strong>'.$cabecalho.'</strong>
        </td>';
	}
	
	$table .= '</tr>';
	if(count($linhas)>0){
		foreach($linhas as $k=>$linha){
			
			if($k%2==0){
				$cor = '#F7F7F7';
			}else{
				$cor = '';
			}
			
			$table .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
			
			$y = 0;
			foreach($linha as $coluna){
				if($arTipo[$y]=='numeric'){
					$estilo = 'align="right" style="color:blue"';
					$coluna = formata_valor($coluna);
				}else{
					$estilo = 'align="left"';
					$coluna = $coluna;
				}
				$table .= ' <td '.$estilo.' >
								'.$coluna.'
							</td>';
				$y++;
			}
			
			$table .= '</tr>';
		}
	}else{
		$table .= '<tr bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';">
						<td align="center" style="color:red" colspan="'.count($arCabecalho).'">
							Nenhum registro encontrado.
						</td>
					</tr>';
	}
	$table .= '</table>';

	echo $table;
    //ver($sql);
}

if($_REQUEST['req']){
    //ver($_POST, d);
	$_REQUEST['req']($_REQUEST);
	die();
}

$sql = "SELECT 
			p.pflcod 
		FROM 
			seguranca.perfilusuario pu 
		INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod 
		WHERE 
			pu.usucpf = '".$_SESSION['usucpf']."' 
			AND p.pflstatus = 'A' 
			AND p.sisid = ".$_SESSION['sisid'];

$perfil = $db->carregarColuna($sql);

$notUngcod = Array();
$esdid = Array();


include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
montaAbaTermoCooperacaoListaUGs();
monta_titulo( 'Tramitação em Lotes', '' );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();

function aguarda(val){
	jQuery('#pesquisar').attr('disabled',val);
	jQuery('#tramita').attr('disabled',val);
	jQuery('#limpar').attr('disabled',val);
	if(val){
		jQuery('#aguardando').show();
	}else{
		jQuery('#aguardando').hide();
	}
}

jQuery(document).ready(function() {

	jQuery('.visualizarTermo').live('click', function(){
		var popUp = window.open('elabrev.php?modulo=principal/termoCooperacao/visualizarTermo&acao=A&aba=proponente&tcpid='+this.id, 'popupTermoCooperacao', 'height=600,width=800,scrollbars=yes,top=50,left=200');
		popUp.focus();
	});

	jQuery('#esdid').click(function(){
		if(jQuery(this).val()){
			jQuery('.tr_acao').show();
		}else{
			jQuery('.tr_acao').hide();
			jQuery('#aedid').val(null);
		}
	});

	jQuery('#esdid').change(function(){

		var esdid = jQuery(this).val();

		if (esdid != '') {
			aguarda(true);
	
			jQuery('#lista').html('');
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "req=montaAcoes&esdid="+esdid,
				async: false,
				success: function(msg){
					jQuery('#tdAcao').html(msg);
					aguarda(false);
				}
			});
		} else {
			jQuery('#tdAcao').html(' ');
		}
	});

	jQuery('#aedid').live('change',function(){
		var aedid = jQuery(this).val();

		if( aedid != '' ){
			aguarda(true);
			
			jQuery('#req').val('lista');

            console.log( "&"+jQuery('#formTramita').serialize() );
            //return false;

			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "&"+jQuery('#formTramita').serialize(),
				async: false,
				success: function(msg){					
					jQuery('#lista').html(msg);
					aguarda(false);
				}
			});
		}else{
			jQuery('#lista').html(' ');
		}
	});

	jQuery('.todos').live('click',function(){
		if( jQuery(this).attr('checked') ){
			jQuery('[name="docid[]"]').attr('checked',true);
		}else{
			jQuery('[name="docid[]"]').attr('checked',false);
		}
	});

	jQuery('#tramita').click(function(){
		
		var selecionado = false;
		
		aguarda(true);
		
		jQuery('.doc').each(function(){
			if(jQuery(this).attr('checked')==true){
				selecionado = true;
			}
		});

		var pendencia = false;

		jQuery('.obrigatorio').each(function(){
			var val = jQuery.trim(jQuery(this).val());
			if( val.length == 0 ){
				jQuery(this).focus();
				pendencia = true;
				aguarda(false);
				return false;
			}
		});
				
		if (pendencia) {
			alert('Campo obrigatório.');
			aguarda(false); 
			return false; 
		}
		
		
		if(jQuery('#aedid').val()=='') {
			alert('Selecione Ação de Tramitação');
			return false;
		}
		
		if (selecionado) {
			
			// VALIDAÇÃO DO COMENTÁRIO
			var exibeComentario=false;
			
			jQuery.ajax({
				type: "POST",
				url: 'elabrev.php?modulo=principal/termoCooperacao/tramitaTermos&acao=A',
				data: "req=verificarComentario&aedid="+jQuery('#aedid').val(),
				async: false,
				success: function(msg){
					if(msg=="TRUE") {
						exibeComentario=true;
					}
				}
			});
			
			if (exibeComentario==true && jQuery('#comentario').val()=='') {
				jQuery('#div_comentario').show();
				jQuery( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
				//aguarda(false);
				return false;
			}
			// FIM - VALIDAÇÃO DO COMENTÁRIO

            if (confirm('Deseja '+jQuery('#aedid option:selected').html()+'?')) {
                jQuery('#req').val('tramitar');
                jQuery('#formTramita').submit();
            }
			
			aguarda(false);
			return false;
			
		}else{
			
			alert('Selecione pelo menos uma solicitação.');
			
			aguarda(false);
			
			return false;
		}
	});

	jQuery('#limpar').click(function(){
		jQuery('.normal').each(function(){
			jQuery(this).val('');
		});
		jQuery('.CampoEstilo').each(function(){
			jQuery(this).val('');
		});
		jQuery('.tr_acao').hide();
		jQuery('.tr_coordenacao').hide();
		jQuery('.tr_advogado').hide();
		jQuery('#lista').html(' ');
		jQuery('#pesquisar').click();
	});


	jQuery('#pesquisar').click(function(){
		
		aguarda(true);

		jQuery('#req').val('lista');
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: jQuery('#formTramita').serialize(),
			async: false,
			success: function(msg){
				jQuery('#lista').html(msg);
				jQuery('#pesquisar').attr('disabled',false);
			}
		});

		aguarda(false);

	});

    jQuery("#extratoDOU").click(function(){
        $(this).remove();
        location.href="elabrev.php?modulo=principal/termoCooperacao/tramitaTermos&acao=A&geraDoc=true";
    });
});

</script>
<style>
	.PopUphidden{display:none;width:500px;height:300px;position:absolute;z-index:0;top:50%;left:50%;margin-top:-150;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;}
</style>
<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>
<form method="post" name="formTramita" id="formTramita" action="">
	<input type="hidden" id="req" name="req" value=""/>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="40%">Situação da Solicitação</td>
			<td>
			<?php 
				$sql = "SELECT DISTINCT
							esdid as codigo, 
							esddsc as descricao
						FROM 	
							workflow.estadodocumento esd
						INNER JOIN workflow.acaoestadodoc 	  		aed ON aed.esdidorigem 	= esd.esdid
						INNER JOIN workflow.estadodocumentoperfil 	esp ON esp.aedid 		= aed.aedid
						INNER JOIN seguranca.perfilusuario 			pus ON pus.pflcod 		= esp.pflcod
						WHERE  
							tpdid='".WF_TPDID_DESCENTRALIZACAO."'
							AND aed.aedstatus = 'A'
							AND esd.esdstatus = 'A' 
							AND pus.usucpf = '".$_SESSION['usucpf']."'
							AND  esd.esdid in ( ".AGUARDANDO_APROVACAO_DIRETORIA.", ".AGUARDANDO_APROVACAO_SECRETARIO.", ".EM_ANALISE_PELA_CGSO.", ".EM_ANALISE_PELA_SPO.", ".EM_DESCENTRALIZACAO.")
						ORDER  BY 
							esddsc";
                //ver($sql, d);
				$db->monta_combo('esdid',$sql,'S','Selecione...','','','','','S', 'esdid');
				
				$displayAcao = ($_REQUEST['esdid']) ? 'table-row' : 'none';
			?>
			</td>
		</tr>
		<tr class="tr_acao" style="display:<?=$displayAcao?>">
			<td class="SubTituloDireita">Ações de Tramitação</td>
			<td id="tdAcao">
				<?php 
				if($_REQUEST['esdid']){
					$sql = "SELECT DISTINCT
								aed.aedid as codigo,
								aed.aeddscrealizar as descricao  
							FROM 	
								workflow.acaoestadodoc aed 
							INNER JOIN workflow.estadodocumentoperfil 	esp ON esp.aedid 	= aed.aedid
							INNER JOIN seguranca.perfilusuario 			pus ON pus.pflcod 	= esp.pflcod
							WHERE  
								esdidorigem = {$_REQUEST['esdid']}
								AND tpdid = '".WF_TPDID_DESCENTRALIZACAO."' 
								--AND pus.usucpf = '".$_SESSION['usucpf']."'
							ORDER  BY 
								esddsc";
					
					$db->monta_combo('aedid',$sql,'S','Selecione...','','','','','S', 'aedid');
				}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2" id="tdacao" >
				<input type="button" id="limpar" value="Limpar campos da pesquisa." />
			</td>
		</tr>
	</table>
	<div id="lista" >
	</div>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloCentro" colspan="2" id="tdacao" >
				<input type="button" id="tramita" value="Tramitar" />
                <?php if (count($_SESSION['elabrev']['extratodou'])) : ?>
                    <input type="button" name="extratoDOU" value="Gerar Extrato DOU" id="extratoDOU">
                <?php endif; ?>
			</td>
		</tr>
	</table>
	<div class="PopUphidden" id="div_comentario">
		<div style="width:100%;text-align:right">
			<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('div_comentario').style.display='none'" />
		</div>
		<div style="padding:5px;overflow:auto;height:250px;" >	
			<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td class="SubTituloCentro">Digite o comentário</td>
				</tr>
				<tr>
					<td align="center"><?=campo_textarea( 'comentario', 'N', 'S', '', '70', '4', '200'); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						<input type="button" name="tramitar2" value="Tramitar" onclick="jQuery('#tramita').click();"> 
						<input type="button" name="fechar" value="Fechar" onclick="document.getElementById('div_comentario').style.display='none';aguarda(false);">
                    </td>
				</tr>
			</table>
		</div>
	</div>
</form>