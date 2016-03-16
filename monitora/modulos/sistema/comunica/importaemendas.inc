<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Adonias Malosso
   Programador: Adonias Malosso (e-mail: malosso@gmail.com)
   Módulo:importaemendas.inc
   Finalidade: permitir importar os dados da base de emendas da Câmara
   */
$modulo=$_REQUEST['modulo'] ;
include APPRAIZ."includes/cabecalho.inc";

if ($_REQUEST['__frmenviado'])
{
	$testaupload = (boolean)$_REQUEST["testaupload"];
	echo "<pre>";
	include SISRAIZ."/modulos/sistema/comunica/processaemendas.inc";
	echo "</pre>";
	exit;
}
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Importação de Dados de Emendas';
monta_titulo($titulo_modulo,'');
$anoorigem=$_REQUEST['anoorigem'];
$anodestino=$_REQUEST['anodestino'];

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<form method="POST" enctype="multipart/form-data" name="formulario">
<input type="hidden" name="modulo" value="<?=$modulo?>">
<input type="hidden" name="__frmenviado" value="0">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
    <tr>
      <td colspan="2">
        <p>O recebimento de dados de Emendas deverá ocorrer uma vez ao ano, após sua aprovação no 
        Congresso Nacional.</p>
        <p>Ao pressionar o botão "Carregar dados", o sistema irá ler o arquivo no formato CSV e fazer 
        uma migração seletiva de dados, incluindo os novos e alterando, se for o caso, os antigos.</p>
        <p>Para gerar o arquivo CSV de importação:
          <ul>
            <li>Acesse a base de Elaboração do orçamento da união - Autógrafo; fornecida pela Câmara dos 
            Deputados.</li>
            <li>Execute a consulta Emendas - Aprovadas por Autor Todas.<li>
            <li>Exporte o resultado como CSV.</i>
            <li>Faça o upload do arquivo no formulário abaixo.</i>            
          </ul>
        </p>
      </td>
	</tr>
	<tr>
	  <td class="subTituloDireita" width="200">Arquivo:</td>
	  <td><input type="file" name="arqEmendas"/></td>
	</tr>
	<tr>
	  <td class="subTituloDireita" width="200">Somente testar o upload?<br/>(as mudanças não serão aplicadas)</td>
	  <td><input type="checkbox" name="testaupload" value="1"></td>
	</tr>
	<tr>
	  <td colspan="2" align="center"><input type="button" class="botao" name="enviar" value="Enviar" onclick="frmEnviar()"></td>
	</tr>
</table>
</form> 
<script language="Javascript">
function frmEnviar	() {
	if( window.confirm( "Este processo pode sobrescrever dados existentes e levar algus minutos.\nConfirma a importação de dados de Emendas ?") ) {
		document.formulario.__frmenviado.value = 1;	
		document.formulario.submit();
	}
	else {
		return false;
	}
}
</script>