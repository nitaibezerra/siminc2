<?php
/* Tratamento de segurança */
include APPRAIZ . "rehuf/classes/Certificado.class.inc";

if (validaVariaveisSistema ()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit ();
}
$permissoes = verificaPerfilRehuf ();
validaAcessoHospital ( $permissoes ['verhospitais'], $_SESSION ['rehuf_var'] ['entid'] );
// Validando os acessos as funções autorizadas
switch ($_REQUEST ['acao']) {
	case 'A' :
		$_FUNCOESAUTORIZADAS = array (
				'atualizarestruturaunidade' => true 
		);
		break;
}
if ($_REQUEST ['requisicao']) {
	if ($_FUNCOESAUTORIZADAS [$_REQUEST ['requisicao']]) {
		$_REQUEST ['requisicao'] ( $_REQUEST );
	}
}

if ($_REQUEST ['requisicaoAjax'] && $_REQUEST ['classe']) {
	$n = new $_REQUEST ['classe'] ();
	$n->$_REQUEST ['requisicaoAjax'] ();
	exit ();
}
/* FIM - Tratamento de segurança */

echo '<br>';

/* montando o menu */

/* montando cabeçalho */
monta_titulo ( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais" );
monta_cabecalho_rehuf ( $_SESSION ['rehuf_var'] ['entid'] );

/* Tela de gerenciamento dos dados especificos */
// Buscando os dados especificos do HOSPITAL
$dadosestruturaunidade = $db->pegaLinha ( "SELECT * FROM rehuf.estruturaunidade WHERE entid = '" . $_SESSION ['rehuf_var'] ['entid'] . "'" );
$esutipo = $dadosestruturaunidade ['esutipo'];
$esuareacontruida = number_format ( $dadosestruturaunidade ['esuareacontruida'], 2, ',', '.' );
$esucontratualizado = (($dadosestruturaunidade ['esucontratualizado'] == "t") ? "sim" : "nao");
$esucertiticado = (($dadosestruturaunidade ['esucertiticado'] == "t") ? "sim" : "nao");
$esugestao = $dadosestruturaunidade ['esugestao'];
$esudataadereebserh = formata_data ( $dadosestruturaunidade ['esudataadereebserh'] );
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<script type="text/javascript">
	function cadastrarCertificado(tipo){
		var formulario = document.formulario;
		formulario.target = 'cadastrarCertificado';
		var janela = window.open( 'rehuf.php?modulo=principal/cadastrarCertificado&acao=A&tipo='+tipo, 'cadastrarCertificado', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
		janela.focus();	
	}

	function excluirCertificado(cerid){
		if(confirm('Deseja realmente excluir o registro?')){
			$.ajax({
				   type: "POST",
				   url: window.location,
				   data: "requisicaoAjax=excluirCertificado&classe=Certificado&cerid="+cerid,
				   success: function(msg){
				   		alert('Registro excluído com sucesso');
				   		location.reload();
				   }
				 });
		}
	}

</script>

<form id="formulario" name='formulario'
	action='?modulo=principal/dadosespecificos&acao=A'
	onsubmit='document.getElementById("btnsubmit").disabled=true;'
	method='post'>
	<input type='hidden' name='requisicao' value=''>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1"
		align="center">
		<tr>
			<td>
				<center>
					<label class="TituloTela" style="color: #000000;">Certificados/Certificações Hospitalares</label>
				</center>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="0" cellpadding="0" align="center">
		<tbody>
			<tr>
				<td class="SubTituloCentro">Hospital de Ensino:</td>
				<td>
					<?php
						$certificado = new Certificado ();
						$certificado->listarCertificados ( "1", $_SESSION ['rehuf_var'] ['entid'] );
					?>
				</td>
			</tr>
			<tr>
				<td>
				</td>
				<td align="left">
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:cadastrarCertificado(1)"><img alt="" src="../imagens/gif_inclui.gif"> Adicionar</a>
				</td>
			</tr>
			<tr>
				<td>
					&nbsp;
				</td>
			</tr>
			<tr>
				<td>
					&nbsp;
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro">Hospital Amigo da Criança:</td>
				<td>
					<?php
						$certificado = new Certificado ();
						$certificado->listarCertificados ( "2", $_SESSION ['rehuf_var'] ['entid'] );
					?>
				</td>
			</tr>
			<tr>
				<td>
				</td>
				<td align="left">
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:cadastrarCertificado(2)"><img alt="" src="../imagens/gif_inclui.gif"> Adicionar</a>
				</td>
			</tr>
			<tr>
				<td>
					&nbsp;
				</td>
			</tr>
			<tr>
				<td>
					&nbsp;
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro">Acreditação/Certificação de Qualidade:</td>
				<td>
					<?php
						$certificado = new Certificado ();
						$certificado->listarCertificados ( "3", $_SESSION ['rehuf_var'] ['entid'] );
					?>
				</td>
			</tr>
			<tr>
				<td>
				</td>
				<td align="left">
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:cadastrarCertificado(3)"><img alt="" src="../imagens/gif_inclui.gif"> Adicionar</a>
				</td>
			</tr>
			<tr>
				<td>
					&nbsp;
				</td>
			</tr>
			<tr>
				<td>
					&nbsp;
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro">Outras:</td>
				<td>
					<?php
						$certificado = new Certificado ();
						$certificado->listarCertificados ( "4", $_SESSION ['rehuf_var'] ['entid'] );
					?>
				</td>
			</tr>
			<tr>
				<td>
				</td>
				<td align="left">
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:cadastrarCertificado(4)"><img alt="" src="../imagens/gif_inclui.gif"> Adicionar</a>
				</td>
			</tr>
			<tr bgcolor="#C0C0C0">
				<td colspan='2' align='center'><input type='button' id='btnsubmit' value='Fechar' onclick="javascript:self.close()"></td>
			</tr>
		</tbody>
	</table>
</form>