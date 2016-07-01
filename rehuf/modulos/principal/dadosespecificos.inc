<?php
/* Tratamento de segurança */
if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}
$permissoes = verificaPerfilRehuf();
validaAcessoHospital($permissoes['verhospitais'], $_SESSION['rehuf_var']['entid']);
// Validando os acessos as funções autorizadas
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('atualizarestruturaunidade'=>true);
	break;
}
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}
/* FIM - Tratamento de segurança */


include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), "/rehuf/rehuf.php?modulo=principal/dadosespecificos&acao=A");

/* montando cabeçalho */
monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);

/* Tela de gerenciamento dos dados especificos */
// Buscando os dados especificos do HOSPITAL
$dadosestruturaunidade = $db->pegaLinha("SELECT * FROM rehuf.estruturaunidade WHERE entid = '".$_SESSION['rehuf_var']['entid']."'");
$esutipo 			= $dadosestruturaunidade['esutipo'];
$esuareacontruida   = number_format($dadosestruturaunidade['esuareacontruida'],2,',','.');
$esucontratualizado = (($dadosestruturaunidade['esucontratualizado']=="t")?"sim":"nao");
$esucertiticado 	= (($dadosestruturaunidade['esucertiticado']=="t")?"sim":"nao");
$esugestao 			= $dadosestruturaunidade['esugestao'];
$esudataadereebserh = formata_data($dadosestruturaunidade['esudataadereebserh']);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script>
function validarDadosEspecificos() {
	if(document.getElementById('esudataadereebserh').value != '') {
		if(!validaData(document.getElementById('esudataadereebserh'))) {
			alert('"Data" é inválida.');
			return false;
		}
	}
	
	document.getElementById('formulario').submit();
	
}

function cadastrarCertificado(){
	var formulario = document.formulario;
	formulario.target = 'relatoriopregaorehuf';
	var janela = window.open( 'rehuf.php?modulo=principal/certificados&acao=A', 'relatoriopregaorehuf', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	janela.focus();
}

</script>

<form id="formulario" name='formulario' action='?modulo=principal/dadosespecificos&acao=A' onsubmit='document.getElementById("btnsubmit").disabled=true;' method='post'>
	<input type='hidden' name='requisicao' value='atualizarestruturaunidade'>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
	<td colspan='2'><strong>Dados gerais do hospital</strong></td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Tipo do hospital :</td>
		<td> 
		<? 
		$thosp = array(0 => array("codigo" => "E", "descricao" => "Especialidade"), 
					   1 => array("codigo" => "G", "descricao" => "Geral"), 
					   2 => array("codigo" => "M", "descricao" => "Maternidade")
					   );
		echo $db->monta_combo('esutipo', $thosp, 'S', 'Selecione', '', '', '', '200', 'N', 'esutipo'); 
		?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Área construída total :</td>
		<td>
		<? 
		echo campo_texto('esuareacontruida', 'N', 'S', 'Área contruída total', 37, 37, "###.###.###,##", "", '', '', 0, '' ); 
		?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Contratualizado :</td>
		<td>
		<? 
		$contr = array(0 => array("codigo" => "sim", "descricao" => "Sim"), 
					   1 => array("codigo" => "nao", "descricao" => "Não") 
					   );
		echo $db->monta_combo('esucontratualizado', $contr, 'S', 'Selecione', '', '', '', '200', 'N', 'esucontratualizado'); 
		?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Certificado :</td>
		<td>
		<? 
		$certi = array(0 => array("codigo" => "sim", "descricao" => "Sim"), 
					   1 => array("codigo" => "nao", "descricao" => "Não") 
					   );
		echo $db->monta_combo('esucertiticado', $certi, 'S', 'Selecione', '', '', '', '200', 'N', 'esucertiticado'); 
		?>
		&nbsp;&nbsp;<u><a href="javascript:cadastrarCertificado()" id="linkCertificado">Adicionar Certificação/Acreditação</a></u>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Gestão :</td>
		<td>
		<? 
		$gesta = array(0 => array("codigo" => "E", "descricao" => "Estadual"), 
					   1 => array("codigo" => "M", "descricao" => "Municipal"),
					   2 => array("codigo" => "F", "descricao" => "Federal") 
					   );
		echo $db->monta_combo('esugestao', $gesta, 'S', 'Selecione', '', '', '', '200', 'N', 'esugestao'); 
		?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Data que aderiram a EBSERH :</td>
		<td>
		<?php
		echo campo_data2('esudataadereebserh', 'N', 'S','Data que aderim a EBSERH', '', '', '', $esudataadereebserh, '', '', 'esudataadereebserh');
		?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td colspan='2' align='center'><input type='button' id='btnsubmit' value='Salvar' onclick="validarDadosEspecificos();"></td>
	</tr>
</table>
</form>
<script type="text/javascript">
jQuery(document).ready(function(){
	if(jQuery('#esucertiticado').val() == "sim"){
		jQuery('#linkCertificado').show();
	}
	jQuery('#esucertiticado').change(function(){
		if($(this).val() != "sim"){
			jQuery('#linkCertificado').hide();
		} else if($(this).val() == "sim"){
			jQuery('#linkCertificado').show();
		}
    });
});

</script>