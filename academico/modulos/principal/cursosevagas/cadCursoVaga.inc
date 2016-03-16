<?php 
$c = new CursosEdital();

switch ($_REQUEST['evento']){
	case 'manterEdital':
		$where = array();
		if ($_POST['edtid']){
			$where['edtid'] = $_POST['edtid'];	
			$paramUrl = "&edtid=" . $_POST['edtid'];
		}
		// Inclui o campo file para ser inserido 
		$_POST['arquivo'] = $_FILES['arquivo'];

		if( !$_POST['arquivo']['name'] ){
			
			print "<script>"
				. "    alert('É necessário anexar ao menos um arquivo!');"
				. "    history.back(-1);"
				. "</script>";
			
			die;
			
		}
		
		$retorno = $c->manterEditalcurso($_POST, $where);
		$paramUrl = is_numeric($retorno) ? "&edtid=" . $retorno : $paramUrl;
?>	
	<html>
		<head>		
		<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
		</head>
		<body></body>
	</html>
<?	
		if ($_POST['salvar/fechar']){
			echo $retorno ?
					('<script>alert(\'Operação realizada com sucesso!\'); window.parent.opener.location.href = window.opener.location; window.close();</script>')	
				 :
					('<script>alert(\'Falha na operação!\nHouve alguma incompatibilidade nos dados enviados.\');</script>');	
?>
			<script type="text/javascript">
			<!--
				lista = $('#listaEditais', window.opener.document);
				$(lista).html('<center><img src="/imagens/carregando.gif"><br>Carregando...</center>');
				$(lista).load('?modulo=principal/cursosevagas/listaEditaisVagas&acao=A', {'evento' : 'listaEditais'}, function (){ window.close(); });
			//-->
			</script>
<?
		}else{
?>
			<script type="text/javascript">
			<!--
				lista = $('#listaEditais', window.opener.document);
				$(lista).html('<center><img src="/imagens/carregando.gif"><br>Carregando...</center>');
				$(lista).load('?modulo=principal/cursosevagas/listaEditaisVagas&acao=A', {'evento' : 'listaEditais'});
			//-->
			</script>
<?
			$retorno ? 
				$db->sucesso($_REQUEST['modulo'], $paramUrl) 
			: 
				redir('', 'Falha na operação!\nHouve alguma incompatibilidade nos dados enviados.');
		}	
		
		
	break;
	case 'DA':
		$dados_arq['arqid'] =$_REQUEST['arqid']; 
		DownloadArquivo($dados_arq);
		exit;
	break;
	case 'EA':
		$paramUrl = "&edtid=" . $_REQUEST['edtid'];
		$retorno = $c->excluirAnexoedital($_REQUEST);
		$retorno ? 
			$db->sucesso($_REQUEST['modulo'], $paramUrl) 
		: 
			redir('', 'Falha na operação!\nHouve alguma incompatibilidade nos dados enviados.');
	break;
}

$dados = $c->carregaEdital($_REQUEST['edtid']);
extract($dados);
?>
<html>
	<head>
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
		<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
 
		<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script language="JavaScript" src="./geral/js/academico.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript">
		d = document;
		function downloadArquivo (evento, arqid, edtid){
			redireciona('?modulo=principal/cursosevagas/cadCursoVaga&acao=A&edtid=' + edtid + '&evento=' + evento + '&arqid=' + arqid)		
		}
		
		function salvarEdital(){
			var txt = '';
			$('input').each(function (){
				$(this).val(trim($(this).val()));
			});
			
			if ($("input[name='edtnumero']").val() == '' ){
				txt = 'O campo Nº do edital deve ser preenchido!';
			}
			
			if ($("input[name='edtdsc']").val() == '' ){
				txt += (txt ? '\n' : '') + 'O campo Nome do Edital deve ser preenchido!';
			}

			if ($("input[name='edtdtcriacao']").val() == '' ){
				txt += (txt ? '\n' : '') + 'O campo Data deve ser preenchido!';
			}

			if ($("input[name='edtnumvagas']").val() == '' ){
				txt += (txt ? '\n' : '') + 'O campo Total de Vagas deve ser preenchido!';
			}

			if ($("input[name='edtano']").val() == '' ){
				txt += (txt ? '\n' : '') + 'O campo Ano deve ser preenchido!';
			}
			
			if (txt != ''){
				alert(txt);
				return false;
			}
			return true;
		}
		
		function fechar(){
			alert('felpe');
			window.close();
		}
		</script>
	</head>
<?
$entid = $_SESSION['academico']['entid'] ? $_SESSION['academico']['entid'] : $_REQUEST['entidunidade'] ;
?>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
<?php
monta_titulo( 'Cadastro de Edital de Oferta de Vagas', obrigatorio() . ' Indica campo obrigatório' );
?>
<form method="POST"  name="formulario" onsubmit="javascript:return salvarEdital();" enctype="multipart/form-data">
	<input type="hidden" name="evento" id="evento" value="manterEdital">
	<input type="hidden" name="edtid" id="edtid" value="<?=$edtid?>">
	<input type="hidden" name="entid" id="entid" value="<?=$entid?>">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="SubTituloDireita">Nº do Edital:</td>
        <td>
		<?=campo_texto('edtnumero','S','S','',10,20,'','');?>
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Nome do Edital:</td>
        <td>
		<?=campo_texto('edtdsc','S','S','',60,100,'','');?>
        </td>
      </tr>
      <tr>
      	<td class="subtitulodireita">Tipo de Curso:</td>
      	<td>
      		<input type="radio" value="G" name="edttipo" id="edttipoG" <?php if( $edttipo == "G" || $edttipo == "" ){ print "checked"; }?>/> Graduação
      		<input type="radio" value="P" name="edttipo" id="edttipoP" <?php if( $edttipo == "P" ){ print "checked"; }?>/> Pós-Graduação
      	</td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Data:</td>
        <td>
        	<?= campo_data2( 'edtdtcriacao', 'S', 'S', '', 'S' ); ?>
        </td>
      </tr>     
      <tr>
        <td align='right' class="SubTituloDireita">Total de Vagas:</td>
        <td>
		<?=campo_texto('edtnumvagas','S','S','',6,10,'##########','');?>
			<font size="1" color="red">Não colocar vagas oferecidas para preenchimento de vagas ociosas</font>
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Ano:</td>
        <td>
		<?=campo_texto('edtano','S','S','',6,4,'','');?>
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Inscrições:</td>
        <td>
		<?= campo_data2( 'edtdtinicioinscricao', 'N', 'S', '', 'S' ) . ' até ' .  campo_data2( 'edtdtfinalinscricao', 'N', 'S', '', 'S' )?>
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Provas:</td>
        <td>
		<?= campo_data2( 'edtdtprovainicio', 'N', 'S', '', 'S' ) . ' até ' .  campo_data2( 'edtdtprovafinal', 'N', 'S', '', 'S' )?>
        </td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Data de Início das Aulas:</td>
        <td>
        	<?= campo_data2( 'edtdtinicioaulas', 'N', 'S', '', 'S' ); ?>
        </td>
      </tr>  
      <tr>
        <td align='right' class="SubTituloDireita">Nº do DOU:</td>
        <td>
		<?=campo_texto('edtnumdiario','N','S','',10,20,'','');?>
        </td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Data do DOU:</td>
        <td>
        	<?= campo_data2( 'edtdtpubldiario', 'N', 'S', '', 'S' ); ?>
        </td>
      </tr>  
      <tr>
        <td align='right' class="SubTituloDireita">Nº da Seção:</td>
        <td>
		<?=campo_texto('edtsecaodiario','N','S','',6,10,'','');?>
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Nº da Página:</td>
        <td>
		<?=campo_texto('edtdiariopagina','N','S','',6,10,'','');?>
        </td>
      </tr>
      <tr>
        <td align='right' colspan="2" class="SubTituloCentro">Arquivos do Edital</td>
      </tr>
	  <tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
		<td>
			<input type="file" name="arquivo" />							
		</td>
	  </tr>      
	  <tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
		<td><?= campo_textarea( 'arqdescricao', 'N', $habil, '', 60, 2, 255 );?></td>
	  </tr>
	  <tr>
	  	<td colspan="2">
	  		<fieldset style="height:128px; background:#fff;">
	  			<legend> Arquivos </legend>
	  			<div id="listArquivo" style="height:123px; overflow: auto;">
				<?$c->listaAnexoedital($edtid); ?>
				</div>
			</fieldset>
		</td>
	  </tr>
	  <tr>
	  	<td colspan="2" class="SubTituloEsquerda">
	  		<center>
			<input name="salvar" type="submit" value="Salvar">
			&nbsp;&nbsp;
			<input name="salvar/fechar" type="submit" value="Salvar/Fechar">
			&nbsp;&nbsp;
			<input type="button" value="Fechar" onclick="javascript:window.parent.opener.location.href = window.opener.location; window.close();">
			</center>
		</td>
	  </tr>	  
</table>
</form>      
</body>
</html>