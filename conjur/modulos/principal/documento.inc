<?php

if( !$_SESSION['conjur_var']['prcid'] ){
	echo "<script>alert('Processo não identificado');window.location='conjur.php?modulo=inicio&acao=S';</script>";
	exit;
}

$sql = "SELECT
			coalesce(a.nudid,0) as nudidanexo, 
			nd.nudnumero
		FROM 
			conjur.processoconjur p
		INNER JOIN conjur.numeracaodocumento nd ON p.prcid = nd.prcid
		LEFT  JOIN conjur.anexos 			  a ON a.nudid = nd.nudid  AND a.anxstatus = 'A'
		WHERE 
			nudstatus = 'A' 
			AND p.prcid='".$_SESSION['conjur_var']['prcid']."' ";

$arDados = $db->carregar($sql);
$arDados = ($arDados) ? $arDados : array();
$boMudaCorAba = true;	
foreach ($arDados as $dados) {
	if($dados['nudnumero']){
		if($dados['nudidanexo'] > 0){
			$boMudaCorAba = true;
		} else {
			$boMudaCorAba = false;
		}
	}
}

if($_POST['requisicao'] == 'filtraComboNumeracao'){
	if($_POST['tpdid']){
		$sql = "SELECT tpdgeranumeracao FROM conjur.tipodocumento WHERE tpdid = '{$_POST['tpdid']}'";
		$tpdgeranumeracao = $db->pegaUm($sql);
		if($tpdgeranumeracao == 'N'){
			echo 'desabilitaCombo';
			die;
		} else {
			$sql = "SELECT 
						nd.nudid as codigo, 
						td.tpddsc || ' - ' || nd.nudnumero as descricao 
					FROM 
						conjur.numeracaodocumento nd
					LEFT JOIN conjur.anexos 	     a ON nd.nudid = a.nudid
					INNER JOIN conjur.tipodocumento td ON nd.tpdid = td.tpdid
					WHERE 
						nudstatus = 'A' 
						AND a.nudid IS NULL 
						AND nd.prcid = '{$_SESSION['conjur_var']['prcid']}' 
						AND nd.tpdid = {$_POST['tpdid']}";
			
			$db->monta_combo('nudid', $sql, 'S', "Selecione...", '', '', '', '160', 'N', 'nudid');
		}
	} else {
		$sql = array();
		$db->monta_combo('nudid', $sql, 'S', "Selecione...", '', '', '', '160', 'N', 'nudid');
	}
	die;
}


if($_REQUEST['requisicao']) {

	if ( $_REQUEST['requisicao'] == 'download' ){
		conjur_download_arquivo($_REQUEST);
	} else {
		$_REQUEST['requisicao']($_REQUEST);
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo "<br/>";

/* Tratamento de segurança */
$permissoes = verificaPerfilConjur('', $_SESSION['conjur']['coiid']);
/* FIM - Tratamento de segurança */
echo montarAbasArray(monta_abas_processo($_SESSION['conjur_var']['tprid']),$_SERVER['REQUEST_URI']);
monta_cabecalho_conjur($_SESSION['conjur_var']['prcid']);
//ver($_SESSION['conjur_var']['prcid']);
?>
<script src="./js/conjur.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script src="../includes/calendario.js"></script>
<script>

	$(document).ready(function(){

		// Serve para mudar a cor da aba documentos, quando existe número gerado e não tem arquivo anexado
		var boMudaCorAba = '<?php echo $boMudaCorAba;?>';
		if(!boMudaCorAba){
			if($('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().next().next().text() != 'Documentos'){
				$('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().css('color','red').css('font-weight','bold');
			} else {
				$('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().next().next().css('color','red').css('font-weight','bold');
			}
			
		}
		
		$('#tpdid').change(function(){
			var data = new Array();
			data.push({name : 'requisicao',  value : 'filtraComboNumeracao'}, 
					  {name : 'tpdid',       value : $(this).val()} 
					 );	
			$.ajax({
				   type		: "POST",
				   url		: "conjur.php?modulo=principal/documento&acao=A",
				   data		: data,
				   async    : false,
				   success	: function(msg){
									if(msg == 'desabilitaCombo'){
										$('#nudid').attr('disabled', true);
										$('#boNumeracaoObrig').val('0');
									} else {
										$('#boNumeracaoObrig').val('1');
					   					$('#divComboNumeracao').html(msg);
									}
							  }
				 });
			
		});
	});
	
	function conjur_DownloadArquivo( id ){
		window.location = '/conjur/conjur.php?modulo=principal/documento&acao=A&requisicao=download&arqid='+id;
	}
	
	function validaForm(){
		var msg = "";

		if($('#arquivo').val() == ''){
			msg += "O campo Arquivo é obrigatório.\n";
		}
		if($('#tpdid').val() == ''){
			msg += "O campo Tipo de Documento é obrigatório.\n";
		}
		if($('#boNumeracaoObrig').val() == '1' && $('#nudid').val() == ''){
			msg += "O Número do Documento é obrigatório. Selecione o número gerado. \n";
			$('#nudid').focus();
		}
		if($('#anxdesc').val() == ''){
			msg += "O campo Descrição é obrigatório.\n";
		}
				
		if(msg){
			alert(msg);
			return false;
		}

		$('#formulario').submit();
		
	}

	function abrePopupDocPendentes()
	{	
		window.open('conjur.php?modulo=principal/popupDocPendentes&acao=A','','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=500,height=200');
	}
	
</script>
<?
if($permissoes['gravar']){
	?>
<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirarquivoconjur" /> 
<input type="hidden" name="anxtipo" value="P" />
<input type="hidden" name="boNumeracaoObrig" id="boNumeracaoObrig" value="0" />
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
	align="center">
	<tr>
		<td class="SubTituloDireita">Arquivo :</td>
		<td><input type="file" name="arquivo" id="arquivo" /> <img border="0"
			title="Indica campo obrigatório." src="../imagens/obrig.gif" /></td>
	</tr>

	<tr>
		<td class="SubTituloDireita">Tipo de documento:</td>
		<td><?php
		$sql = "SELECT
							 	tipodoc.tpdid  as codigo    ,
								tipodoc.tpddsc as descricao  
							FROM
								conjur.tipodocumento as tipodoc
							WHERE
								tipodoc.tpdstatus = 'A'
							ORDER BY 
								tipodoc.tpddsc ASC";
		$db->monta_combo('tpdid', $sql, 'S', "Selecione...", '', '', '', '160', 'S', 'tpdid');
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Númeração de documento:</td>
		<td>
		<div id="divComboNumeracao"><?php
		$sql = array();
		$db->monta_combo('nudid', $sql, 'S', "Selecione...", '', '', '', '160', 'N', 'nudid');
		?></div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Ementa:</td>
		<td><?= campo_textarea('anxdesc', 'S', 'S', '', 80, 8, 1000 ); ?></td>
	</tr>
	<tr style="background-color: #cccccc">
		<td style="vertical-align: top; width: 25%">&nbsp;</td>
		<td><input type="button" name="botao" value="Salvar" onclick="validaForm()" /><input type="button" name="botao" value="Documentos Pendentes" onclick="abrePopupDocPendentes()" <?php echo ($boMudaCorAba) ? "disabled=\"disabled\"" : ""; ?> /></td>
	</tr>
</table>
</form>
		<?
}
$admin = '';
$pflcods = Array(PRF_SUPERUSUARIO,PRF_ADMINISTRADOR,PRF_TECNICO_ADM);
if( possuiPerfil( $pflcods ) ){
	$admin = "'<center>
				<a href=\"#\" onclick=\"javascript:Excluir(\'?modulo=principal/documento&acao=A&requisicao=removerdocumento&anxid='||anx.anxid||'&arqid='||arq.arqid||'\',\'Deseja realmente excluir o documento?\');\">
					<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\">
			  	</a>
			  	<a href=\"#\" onclick=\"javascript:Excluir(\'?modulo=principal/documento&acao=A&requisicao=desvinculardocumento&anxid='||anx.anxid||'&arqid='||arq.arqid||'\',\'Deseja realmente desvincular o documento?\');\">
					<img src=\"/imagens/reject2.gif\" border=0 title=\"Desvincular\">
			  	</a></center>' as acao,";
	$cabecalho = array("Ação", "Data Inclusão", "Tipo Documento", "Número do Documento", "Nome Arquivo", "Descrição Arquivo", "Responsável");
}else{
	$cabecalho = array("Data Inclusão", "Tipo Documento", "Número do Documento", "Nome Arquivo", "Descrição Arquivo", "Responsável");
}

// solucao temporaria - desativa anexos que não possuem arquivos consolidados
	$sql = " SELECT anx.anxid, a.* 
			 FROM conjur.anexos anx
			 INNER JOIN public.arquivo a ON a.arqid = anx.arqid
			 WHERE anx.prcid = '".$_SESSION['conjur_var']['prcid']."' AND anx.anxtipo='P' AND anx.anxstatus='A' ";
	// ver($sql,d);
	$listaDocumentos = $db->carregar( $sql );

	if( !empty($listaDocumentos) )
	foreach ($listaDocumentos as $key => $value) {

		if( !file_exists(APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($value['arqid']/1000) .'/'.$value['arqid']) ){
			$sql = " UPDATE conjur.anexos SET anxstatus = 'I' WHERE anxid = ".$value['anxid']." ";
			$db->executar($sql);

			$dadosDoInforme = array(
				'anxid' => $value['anxid'],
				'prcid' => $_SESSION['conjur_var']['prcid'],
				'arqid' => $value['arqid']
			);
//			infromaTiErroArquivoAnexoConjur( $dadosDoInforme );
		}
	}
	$db->commit();
// --

$sql = "SELECT	
			{$admin}
			to_char(anx.anxdtinclusao, 'dd/mm/yyyy'),
			--tpa.tpadsc,
			td.tpddsc,
			case when nd.nudnumero is null then ' - '
			     else nd.nudnumero::varchar
			end as nudnumero,
			'<a style=\"cursor: pointer; color: blue;\" onclick=\"conjur_DownloadArquivo(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>' as nomearquivo,
			anx.anxdesc,
			usu.usunome
		FROM 
			conjur.anexos anx 
		LEFT JOIN conjur.tipodocumento td ON anx.tpdid = td.tpdid
		LEFT JOIN conjur.numeracaodocumento nd ON td.tpdid = nd.tpdid AND nd.nudstatus = 'A' and nd.prcid = anx.prcid and nd.nudid = anx.nudid
		LEFT JOIN public.arquivo arq ON arq.arqid = anx.arqid 
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = arq.usucpf  
		WHERE anx.prcid='".$_SESSION['conjur_var']['prcid']."' AND anx.anxtipo='P' AND anx.anxstatus='A'";

$db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '', '' );

?>