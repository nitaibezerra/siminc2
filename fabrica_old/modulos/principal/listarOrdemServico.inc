<?
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']();
	exit;
}


if($_REQUEST['tpcancela']) {
	$ok = regraCancelaOS( $_REQUEST['odsid'], $_REQUEST['tpcancela'] );

	if($ok){
		echo "<script>alert('Operação Efetuada com Sucesso!')</script>";
	}
	else{
		echo "<script>alert('Erro, Favor entrar em contato com o administrador do sistema!')</script>";
	}

	die("<script>location.href='fabrica.php?modulo=principal/listarOrdemServico&acao=A&pesquisar=".$_REQUEST['pesquisar']."&odsid=".$_REQUEST['odsid']."';</script>");
}



//trata combo_popup para paginação e ordenação por coluna
		//campo sistemas
		if(!$_POST['numero'] && !$_POST['ordemlista']){
			if($_POST['sidid'][0]){
				for($i=0; $i<count($_POST['sidid']);  $i++){
					$sididx .= "|" . $_POST['sidid'][$i];
				}
				$_SESSION['sididaux'] = substr($sididx, 1);
			}
			else{
				$_SESSION['sididaux'] = "";
			}
		}
		if($_SESSION['sididaux']){
			$sididx = str_replace("|","','",$_SESSION['sididaux']);
			//parametro para query
			$sqlx = "select
						sidid as codigo,
						siddescricao as descricao
					from
						demandas.sistemadetalhe
					Where sidid in ('".$sididx."')
					order by siddescricao";
			$_POST['sidid'] = $db->carregarColuna($sqlx);
			//preenche combopopup
			$sidid = $db->carregar($sqlx);
		}
//fim trata combo_popup





$arrPerfil = pegaPerfilGeral();


//permissao PERFIL_PREPOSTO, PERFIL_ESPECIALISTA_SQUADRA
$botoesContagem = 1;
if( (in_array(PERFIL_PREPOSTO,$arrPerfil) || in_array(PERFIL_ESPECIALISTA_SQUADRA,$arrPerfil)) && !in_array(PERFIL_SUPER_USUARIO,$arrPerfil)){
	$andPreposto = " AND TOSID = 1 ";
	$botoesContagem = 0;
	$filtro[] = "tos.tosid in (1,2,3)";
	$filtro[] = "os.odscontratada = true";
}

//permissao PERFIL_CONTAGEM_PF -> EMPRESA CONTRATADA
if(in_array(PERFIL_CONTAGEM_PF,$arrPerfil) && !in_array(PERFIL_SUPER_USUARIO,$arrPerfil)){
	$filtro[] = "tos.tosid in (2,3)";
	$_REQUEST['pesquisar'] = "Pesquisar";
}

//Se o perfil for de alguém que represente a empresa que fará a contagem de P.F., deve-se exibir apenas as OS referentes à contagem de P.F.
if(is_array($arrPerfil) && in_array(PERFIL_CONTAGEM_PF,$arrPerfil) && !$db->testa_superuser()){
	$filtro[] = "tos.tosid != 1";
	$campo_situacao = ",ed2.esddsc as esddsc";
}


//filtro PERFIL_REQUISITANTE
if(in_array(PERFIL_REQUISITANTE,$arrPerfil) && !in_array(PERFIL_SUPER_USUARIO,$arrPerfil)){
	$filtro[] = "ss.usucpfrequisitante = '".$_SESSION['usucpf']."'";
}


//filtra demandas filhas
if(!$_REQUEST['odsidAjax'] && !$_REQUEST['pesquisar']) {
	$filtro[] = " os.odsidpai is null ";
}

if($_REQUEST['odsidAjax']){
	$filtro[] = " os.odsidpai = ". $_REQUEST['odsidAjax'];
}


if( isset($_REQUEST['pesquisar']) ){

	if($_REQUEST['odsid']){
		$filtro[] = "os.odsid = '{$_REQUEST['odsid']}'";

	}
	if($_REQUEST['odsdtprevinicio']){
		$filtro[] = "os.odsdtprevinicio = '".formata_data_sql($_REQUEST['odsdtprevinicio'])."'";

	}
	if($_REQUEST['odsdtprevtermino']){
		$filtro[] = "os.odsdtprevtermino = '".formata_data_sql($_REQUEST['odsdtprevtermino'])."'";

	}
	if($_REQUEST['odsdetalhamento']){
		$filtro[] = "os.odsdetalhamento ILIKE '%{$_REQUEST['odsdetalhamento']}%'";

	}
	if( $_REQUEST['esdid'] && $_REQUEST['esdid'] != 'Criada' )
	{
		if($_REQUEST['tosid'] == '1'){
			//$filtro[] = ($_REQUEST['esdid']== WF_ESTADO_CPF_AGUARDANDO_CONTAGEM) ? "ed2.esdid = '{$_REQUEST['esdid']}'" : "ed.esdid = '{$_REQUEST['esdid']}'";
			$filtro[] =  "ed.esdid in ({$_REQUEST['esdid']})";
		}
		elseif($_REQUEST['tosid'] == '2' || $_REQUEST['tosid'] == '3'){
			$filtro[] =  "ed2.esdid in ({$_REQUEST['esdid']})";
		}
		else{
			$filtro[] =  "( (ed2.esdid in ({$_REQUEST['esdid']}) and tos.tosid in (2,3)) OR (ed.esdid in ({$_REQUEST['esdid']}) and  tos.tosid in (1)) )";
		}
	}
	if($_REQUEST['tosid']){
		$filtro[] = "tos.tosid = '{$_REQUEST['tosid']}'";
	}

	if($_POST['sidid'][0] != "" && $_POST['sidid'] != "Array")
	{
		$_POST['sidid'] = "'" . implode("','",$_POST['sidid']) . "'";
		$filtro[] = "ss.sidid IN (".$_POST['sidid'].") ";
	}
        
}

$sqlLista = "SELECT
			'<div style=\"white-space: nowrap;\">'
			||
			(CASE WHEN tos.tosid = ".TIPO_OS_GERAL." THEN '<img src=../imagens/alterar.gif title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.open(\'fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='||os.odsid||'\&ansid='||fas.ansid||'\',\'Observacoes\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\">'
	 			  ELSE
	 			  	   (CASE WHEN ed2.esdid != ".WF_ESTADO_CPF_CANCELADA." AND 1=$botoesContagem THEN

	 			  	   			'<img src=../imagens/alterar.gif title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\'\">' --'&nbsp;<img title=\"Cancelar OS PF.\" style=cursor:pointer; src=../imagens/codigo3.gif onclick=\"cancelaOS('||os.odsid||',3);\">'
	 			  	   		 ELSE
	 			  	   		 	'<img src=../imagens/alterar.gif title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\'\">'
	 			  	   	END )
			 END)
			 || (CASE WHEN (ed.esdid = ".WF_ESTADO_OS_FINALIZADA." OR ed2.esdid = ".WF_ESTADO_CPF_FINALIZADA.") THEN '&nbsp;<img title=\"Consultar O.S.\" src=../imagens/consultar.gif style=cursor:pointer; onclick=\"window.open(\'fabrica.php?modulo=principal/verDadosOs&acao=A&odsid='||os.odsid||'\',\'OS\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\">' ELSE '' END)
			 || (CASE WHEN (ed.esdid = ".WF_ESTADO_OS_VERIFICACAO." OR ed2.esdid = ".WF_ESTADO_CPF_AVALIACAO.") AND 1=$botoesContagem THEN '&nbsp;<img title=\"Estimar Contagem de P.F.\" style=cursor:pointer; src=../imagens/editar_nome.gif onclick=\"window.open(\'fabrica.php?modulo=principal/popup/contagemPontoFuncao&acao=A&tosid=".TIPO_OS_CONTAGEM_ESTIMADA."&odsidpai='||os.odsid||'\',\'CONTAGEMPF\',\'scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no\');\">' ELSE '' END)
			 || (CASE WHEN (ed.esdid = ".WF_ESTADO_OS_VERIFICACAO." OR ed2.esdid = ".WF_ESTADO_CPF_AVALIACAO.") AND 1=$botoesContagem THEN '&nbsp;<img title=\"Detalhar Contagem de P.F.\" style=cursor:pointer; src=../imagens/editar_nome_vermelho.gif onclick=\"window.open(\'fabrica.php?modulo=principal/popup/contagemPontoFuncao&acao=A&tosid=".TIPO_OS_CONTAGEM_DETALHADA."&odsidpai='||os.odsid||'\',\'DETALHARPF\',\'scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no\');\">' ELSE '' END)
			 || (CASE WHEN osp.contador > 0 THEN '&nbsp;&nbsp;<a href=\"javascript:void(0);\" onclick=\"montaSubLista('|| os.odsid ||')\"><img id=\"img_mais_'|| os.odsid ||'\" src=\"../imagens/mais.gif\" border=0 title=\"Abrir lista OSs filhas\"></a> <a href=\"javascript:void(0);\" onclick=\"desmontaSubLista('|| os.odsid ||')\"><img id=\"img_menos_'|| os.odsid ||'\" src=\"../imagens/menos.gif\" border=0 title=\"Fechar lista OSs filhas\" style=\"display:none\"></a>' ELSE '' END)
			 || '</div>' as acao,

            '<span style= \"display:none;\">'||lpad(os.odsid::text, 8, '0')||'</span><div style=\"white-space: nowrap; color: #0066CC;\">'
            ||
            (CASE WHEN tos.tosid = ".TIPO_OS_GERAL." THEN '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.open(\'fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='||os.odsid||'\&ansid='||fas.ansid||'\',\'Observacoes\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\">'||os.odsid||'</p>'
                  ELSE
                       (CASE WHEN ed2.esdid != ".WF_ESTADO_CPF_CANCELADA." AND 1=$botoesContagem THEN
                                '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\'\">'||os.odsid||'</p>'
                             ELSE
                                '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\'\">'||os.odsid||'</p>'
                        END )
             END)
             || '</div>' AS odsid,

            '<span style= \"display:none;\">'||lpad(ss.scsid::text, 8, '0')||'</span><div style=\"white-space: nowrap; color: #0066CC;\">'
                || '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||ss.scsid||'&ansid='||fas.ansid || '\'\">'||ss.scsid||'</p>'                
                || '</div>' AS scsid,
			os.odsdetalhamento,
			'<span style= \"display:none;\">' || os.odsdtprevinicio  ||'</span>' || to_char(os.odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio,
			 '<input type=\"hidden\" value=\"'|| to_char(os.odsdtprevtermino, 'YYYYMMDD') ||'\" />'||
			(CASE WHEN ed.esddsc = 'Finalizada' OR ed2.esddsc = 'Finalizada'  THEN
			 			'<font title=\"O.S. Finalizada!\">' ||  to_char(os.odsdtprevtermino, 'DD/MM/YYYY') || '<font>'
		    ELSE
		    '<span style= \"display:none;\">' || to_char(os.odsdtprevtermino, 'DD/MM/YYYY')  ||'</span>' ||
			   (CASE WHEN os.odsdtprevtermino is not null THEN
							(CASE WHEN  to_char(os.odsdtprevtermino::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
					 			THEN '<font color=\"#FBB917\" title=\"\">' || to_char(os.odsdtprevtermino, 'DD/MM/YYYY') || '</font>'
					 			ELSE
									(CASE WHEN os.odsdtprevtermino < CURRENT_DATE THEN
												'<font color=\"red\" title=\"\">' || to_char(os.odsdtprevtermino, 'DD/MM/YYYY') || '</font>'
							 			  ELSE
							 			  		'<font color=\"green\" title=\"\">'  || to_char(os.odsdtprevtermino, 'DD/MM/YYYY') || '</font>'
							 		END)
					 		END)
				END)
		    END) as dataprev,
			os.odsqtdpfestimada,
			os.odsqtdpfdetalhada,
			tosdsc,                        
			(CASE WHEN tos.tosid = ".TIPO_OS_GERAL."
				THEN ed.esddsc
				ELSE ed2.esddsc
			END)
			||
			'</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=10 style=\"padding-left:20px;\" id=\"td_' || os.odsid || '\" ></td></tr>' as esddsc
		FROM fabrica.ordemservico os
		LEFT JOIN ( select odsidpai,  count(odsid) as contador from fabrica.ordemservico group by odsidpai ) osp ON osp.odsidpai = os.odsid
		LEFT JOIN fabrica.tipoordemservico tos ON tos.tosid=os.tosid
		LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid=os.scsid
		LEFT JOIN workflow.documento d ON d.docid=os.docid
		LEFT JOIN workflow.estadodocumento ed ON ed.esdid=d.esdid
		LEFT JOIN workflow.documento d2 ON d2.docid=os.docidpf
		LEFT JOIN workflow.estadodocumento ed2 ON ed2.esdid=d2.esdid
		LEFT JOIN demandas.sistemadetalhe sid ON sid.sidid=ss.sidid
		LEFT JOIN  workflow.documento as wkd on wkd.docid = ss.docid
		LEFT JOIN  fabrica.analisesolicitacao as fas on fas.scsid=ss.scsid";

        


		$sqlLista .= " WHERE tos.tosstatus = 'A' $wherePausa
		".(($filtro) ? " AND " . implode(" AND ", $filtro) :"" )."
		order by os.scsid DESC, tos.tosid, odsdetalhamento";
		
                
//Ajax
if ($_REQUEST['odsidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
  	explodeSubOS($sqlLista);
  	exit;
}



function explodeSubOS($sql)
{
	global $db;

	$cabecalho     = array("Ação","Nº OS","Nº SS","Detalhamento", "Prev. Início", "Prev. Término", "Qtd. PF. Estimada", "Qtd. PF. Detalhada", "Tipo de OS","Situação");
	$tamanho       = array("5%","5%","5%","30%", "10%", "10%", "5%", "5%", "10%", "5%");
	$alinhamento   = array("center","center","center","left", "center", "center", "center", "center", "center", "center");
	$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2,"",$tamanho,$alinhamento);
}







//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

$titulo_modulo = "Listar Ordem de Serviço";
monta_titulo($titulo_modulo, '');

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>
<script type="text/javascript">

function cancelaOS(odsid, tipo){

	if(confirm('Deseja realmente cancelar esta OS?')){
		location.href='fabrica.php?modulo=principal/listarOrdemServico&acao=A&tpcancela='+tipo+'&odsid='+odsid;
	}

}

function limparCampos(){
	$('input:not(:submit,:hidden,:button,[readonly=""],[readonly="readonly"]),textarea,select').val("");
	$('#radioSituacao').click();
	//$('input[name="situacaoPausada[]"]').attr('checked',false);
}


function submeteFormulario()
{
	$('#sidid option').each(function()
	{
		$(this).attr('selected',true);
	});
	$('#form_listar_contratos').submit();
}


function montaSubLista(odsid)
{
	td 	   = document.getElementById('td_'+odsid);
	img_mais   = document.getElementById('img_mais_'+odsid);
	img_menos   = document.getElementById('img_menos_'+odsid);


	// Faz uma requisição ajax, via POST
	$.ajax({type: "POST",
	   		url: "fabrica.php?modulo=principal/listarOrdemServico&acao=A",
	   		data: "odsidAjax="+odsid,
	   		success: function(msg){

				td.style.display = '';
				//td.style.padding = '5px';
				img_mais.style.display = 'none';
				img_menos.style.display = '';
				td.innerHTML = msg;
	   		}
	 });

}

function desmontaSubLista(odsid)
{
	td 	   = document.getElementById('td_'+odsid);
	img_mais   = document.getElementById('img_mais_'+odsid);
	img_menos   = document.getElementById('img_menos_'+odsid);

	td.innerHTML = '';
	td.style.display = 'none';
	img_mais.style.display = '';
	img_menos.style.display = 'none';

}


$(function() {
	$("#numeroOrdemServico").keydown(function(event) {
		var valorDoCampo = $.trim($(this).val());
		if (valorDoCampo != '' && event.keyCode == '13') {
			$("#form_listar_contratos").submit();
		}
	});
        
        
        
});

$(document).ready( function(){
    $("#numeroOrdemServico").focus();
} );

</script>

<form name="formulario" id="form_listar_contratos" method="post" action="">

	<input type="hidden" name="pesquisar" value="pesquisar">

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%">Número</td>
			<td><input id="numeroOrdemServico" class="normal" type="text" 
					onkeyup="this.value=mascaraglobal('[#]',this.value);" name="odsid" value="<?php echo $_REQUEST['odsid']; ?>"></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Previsão de Início</td>
			<td><?php
					echo campo_data2('odsdtprevinicio','N', 'S', 'Expectativa de atendimento', 'S', '', '', formata_data_sql($_REQUEST['odsdtprevinicio']) );
		  		?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Previsão de Término</td>
			<td><?php
					echo campo_data2('odsdtprevtermino','N', 'S', 'Expectativa de término', 'S', '', '', formata_data_sql($_REQUEST['odsdtprevtermino']) );
				?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Detalhamento</td>
			<td><input type="text" class="normal" name="odsdetalhamento" value="<?php echo $_REQUEST['odsdetalhamento']; ?>"></td>
		</tr>
		<?php if((is_array($arrPerfil) && !in_array(PERFIL_CONTAGEM_PF,$arrPerfil)) || $db->testa_superuser()): ?>
			<tr>
				<td class="SubtituloDireita" >Tipo de OS:</td>
				<td>
				<?php
						  $sql = "SELECT
										tosid as codigo,
										tosdsc as descricao
									FROM fabrica.tipoordemservico
									WHERE tosstatus = 'A'
									$andPreposto
									ORDER BY descricao";
			  			   $db->monta_combo('tosid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'tosid', '', $_REQUEST['tosid']);
			  	?>
				</td>
			</tr>
		<?php endif; ?>
		 <tr>
			<td class="SubtituloDireita" width="25%">Situação</td>
			<td><?php
	  		?>
				<select id="esdid" style="width: auto" class="CampoEstilo" name="esdid">
					<option value="">Selecione</option>
					<option value="273" <?php if($_REQUEST['esdid'] == '273') echo "selected";?>>Aguardando Contagem</option>
                    <option value="263" <?php if($_REQUEST['esdid'] == '263') echo "selected";?>>Aguardando Homologação do Gestor</option>
					<option value="<?php 
					echo WF_ESTADO_OS_AGUARDANDO_PAGAMENTO.','.WF_ESTADO_CPF_AGUARDANDO_PAGAMENTO; 
					?>" <?php if($_REQUEST['esdid'] == WF_ESTADO_OS_AGUARDANDO_PAGAMENTO.','.WF_ESTADO_CPF_AGUARDANDO_PAGAMENTO) echo "selected";?>>Aguardando Pagamento</option>
                    <option value="302" <?php if($_REQUEST['esdid'] == '302') echo "selected";?>>Cancelada Com Custo</option>
					<option value="303" <?php if($_REQUEST['esdid'] == '303') echo "selected";?>>Cancelada Sem Custo</option>
					<option value="254" <?php if($_REQUEST['esdid'] == '254') echo "selected";?>>Criada</option>
					<option value="<?php 
					echo WF_ESTADO_OS_DIVERGENCIA.','.WF_ESTADO_CPF_DIVERGENCIA; 
					?>" <?php if($_REQUEST['esdid'] == WF_ESTADO_OS_DIVERGENCIA.','.WF_ESTADO_CPF_DIVERGENCIA ) echo "selected";?>>Divergência</option>
					<option value="264,275" <?php if($_REQUEST['esdid'] == '264,275') echo "selected";?>>Em Aprovação</option>
					<option value="256,274" <?php if($_REQUEST['esdid'] == '256,274') echo "selected";?>>Em Avaliação</option>
					<option value="255" <?php if($_REQUEST['esdid'] == '255') echo "selected";?>>Em Execução</option>
					<option value="<?php 
					echo WF_ESTADO_OS_PAUSA; 
					?>" <?php if($_REQUEST['esdid'] == WF_ESTADO_OS_PAUSA ) echo "selected";?>>Em Pausa</option>
					<option value="<?php echo WF_ESTADO_CPF_REVISAO; ?>" <?php if($_REQUEST['esdid'] == WF_ESTADO_CPF_REVISAO) echo "selected";?>>Em Revisão</option>
					<option value="262" <?php if($_REQUEST['esdid'] == '262') echo "selected";?>>Em Validação</option>
					<option value="257,277" <?php if($_REQUEST['esdid'] == '257,277') echo "selected";?>>Finalizada</option>
					<option value="272" <?php if($_REQUEST['esdid'] == '272') echo "selected";?>>Pendente</option>
				</select>
			</td>
		</tr>
		<!--
		<tr>
			<td class="SubtituloDireita" width="25%">Pausado</td>
			<td>
				<input type="checkbox" name="situacaoPausada[]" id="situacaoPausada" value="SIM" <?php echo $situacaoPausadaChecked ?>/><span>SIM</span>
			</td>
		</tr>
		-->
		<tr>
			<td class="SubtituloDireita" width="25%">Sistema</td>
			<td>
				<?php
					$sql_combo = "select
								sidid as codigo,
								siddescricao as descricao
							from
								demandas.sistemadetalhe
							where
								lower(sidstatus) = 'a'
							order by
								siddescricao";
					$wherePesq = array(
								array(
										"codigo" 	=> "siddescricao",
										"descricao" => "Sistema:",
										"numeric"	=> false
							   		  )
							   );
					combo_popup( 'sidid', $sql_combo, 'Selecione o(s) Sistema(s)', '400x400', 0, array(), '', 'S', true, true, 5, 400, null, null, null, $wherePesq, null, true, false, null, true);
				?>
			</td>
		</tr>
		<tr>
		<td></td>
			<td>
				<input type="submit" value="Pesquisar" onclick="submeteFormulario();">
				&nbsp;
				<input type="button" value="Limpar" id="limpar" onclick="limparCampos();">
			</td>
		</tr>
	</table>

</form>

<?php

$cabecalho     = array("Ação","Nº OS","Nº SS","Detalhamento", "Prev. Início", "Prev. Término", "Qtd. PF. Estimada", "Qtd. PF. Detalhada", "Tipo de OS", 'Situação');
$tamanho       = array("5%","5%","5%","30%", "10%", "10%", "5%", "5%", "5%","10%");
$alinhamento   = array("center","center","center","left", "center", "center", "center", "center", "center", "center");
$db->monta_lista($sqlLista,$cabecalho,100,5,'N','center',$par2,"",$tamanho,$alinhamento);

?>