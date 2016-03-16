<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:consusuario.inc
   Finalidade: permitir a consulta no cadastro de usuários do simec
 */
$modulo=$_REQUEST['modulo'] ;
$encontrou = false;

if ($_REQUEST['act']== 'consultar')
{
   header("Location:elabrev.php?modulo=sistema/usuario/cadusuario&acao=C&usucpf=".$_REQUEST['usucpf']);
    exit();
}
if ($_REQUEST['act']== 'alterar')
{
   header("Location:elabrev.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf=".$_REQUEST['usucpf']);
    exit();
}
include APPRAIZ."includes/cabecalho.inc";
?>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
if ($_REQUEST['acao']== 'A') $titulo_modulo='Alterar o Cadastro de Usuários';
if ($_REQUEST['acao']== 'C') $titulo_modulo='Consultar o Cadastro de Usuários';
monta_titulo($titulo_modulo,'');
?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form method="POST" name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
  <tr>
    <td align='right' class="SubTituloDireita">CPF (ou parte do CPF):</td>
    <td><?=campo_texto('usucpf','','','',16,14,'###.###.###-##','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Nome completo (ou parte do nome):</td>
    <td><?$sqlLiveSearchUsu = "SELECT usunome AS valor, usunome AS descricao FROM seguranca.usuario WHERE usunome ILIKE '%s%' ORDER BY usunome limit 10";?>
    <?=campo_texto_ajax($sqlLiveSearchUsu, "usunome", "usunome", "Nome", "", 50, 50, '', 'left')?></td>
  </tr>
  <?if ($_REQUEST['acao']== 'A'){
  $usustatus=$_REQUEST['usustatus'];?>
  <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Status:</td>
        <td>
            <input type="radio" name="usustatus" value="" <?=($usustatus==''?"CHECKED":"")?>> Qualquer
            <input type="radio" name="usustatus" value="A" <?=($usustatus=='A'?"CHECKED":"")?>> Ativo
            <input type="radio" name="usustatus" value="I" <?=($usustatus=='I'?"CHECKED":"")?>> Inativo
            <input type="radio" name="usustatus" value="C" <?=($usustatus=='C'?"CHECKED":"")?>> Cancelado
            <input type="radio" name="usustatus" value="X" <?=($usustatus=='X'?"CHECKED":"")?>> Aguardando aprovação
        </td>
</tr>
  
  <?}?>
  <tr bgcolor="#C0C0C0">
    <td ></td>
    <td><input type='button' class="botao" name='consultar' value='Consultar' onclick="ProcuraUsuario()"></td>
  </tr>
</form>
</table>
<?
 unset($nlinhas);
 unset($sql);

$cabecalho = array('Ações','CPF','Nome Completo','Órgão','Data de Inscrição (ano/mes/dia)');
if ($_REQUEST['usunome']) $texto = " and u.usunome like '%".str_to_upper($_REQUEST['usunome'])."%'";
if ($_REQUEST['usucpf']) $texto.= " and u.usucpf like '%".str_to_upper(corrige_cpf($_REQUEST['usucpf']))."%'";
if ($_REQUEST['usustatus']) $texto.= " and u.usustatus = '".$_REQUEST['usustatus']."'";

if ($_REQUEST['acao']== 'A')
{
    $sql= "select distinct '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Cadastro \" onclick=\"alterar_usu(' || '\'' || u.usucpf || '\'' || ')\">' as acao,  u.usucpf, u.usunome, o.orgdsc,to_char(u.usudatainc,'YYYY/mm/dd') as data from usuario u, orgao o where u.orgcod=o.orgcod  $texto order by u.usunome";
}
if ($_REQUEST['acao']=='C')
{
    $sql= "select  distinct '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Cadastro \" onclick=\"consultar_usu(' || '\'' || u.usucpf || '\'' || ')\">' as acao,  u.usucpf, u.usunome, o.orgdsc,to_char(u.usudatainc,'YYYY/mm/dd') as data from usuario u, orgao o where u.orgcod=o.orgcod  $texto order by u.usunome";
}
dbg($sql);
if ($_REQUEST['usucpf'] or $_REQUEST['usunome'] or  $_REQUEST['usustatus']) $db->monta_lista($sql,$cabecalho,100,20,'','','');
?>


<script>
    function ProcuraUsuario() {
	 if (document.formulario.usunome)
	 {
	    document.formulario.act.value = "procura";
	    document.formulario.submit()
	 }
    }
    function consultar_usu(cod){
        document.formulario.act.value = "consultar";
        document.formulario.usucpf.value = cod;
	    document.formulario.submit()
    }
    function alterar_usu(cod){
        document.formulario.act.value = "alterar";
        document.formulario.usucpf.value = cod;
	    document.formulario.submit()
    }
</script>

