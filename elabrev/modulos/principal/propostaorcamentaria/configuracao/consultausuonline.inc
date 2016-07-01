<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com)
   Módulo:conMontante.inc
   Finalidade: permitir a consulta de usuários on-line
 */
$modulo=$_REQUEST['modulo'] ;
$encontrou = false;
$qtdMontante = 0;
$descricao = $_REQUEST['descricao'];

if ($_REQUEST['act']== 'excluir')
{
	$qtdMontante=(integer)$db->pegaUm( "select count(mtmvlrlimite) as contador from montantematriz where ppoid = ".$_REQUEST['acaid']."");
	if ($qtdMontante > 0)
	{
?>
	<script>
			alert('Não foi possível remover, pois existem montantes cadastrados para a Proposta!'); 
            history.back();
//			location.href='elabrev.php?modulo=principal/propostaorcamentaria/conMontante&acao=A'; 
	</script>
<?	   
	}
	else
	{
	$sql2="delete from elabrev.limiteppo where ppoid = ".$_REQUEST['acaid'];
	$res = $db->executar($sql2);
	$sql2="delete from elabrev.propostaorcamento where ppoid = ".$_REQUEST['acaid'];
	$res = $db->executar($sql2);
    $db->commit();
?>
	<script>alert('Exclusão efetuada com sucesso!');</script>
<?			
		
	}
}

if ($_REQUEST['act']== 'consultar')
{
   header("Location:elabrev.php?modulo=principal/propostaorcamentaria/montante&acao=A&idproposta=".$_REQUEST['acaid']);
    exit();
}
if ($_REQUEST['act']== 'alterar')
{
   header("Location:elabrev.php?modulo=principal/propostaorcamentaria/montante&acao=A&idproposta=".$_REQUEST['acaid']);
    exit();
}
?>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
if ($_REQUEST['acao']== 'A') $titulo_modulo='Consultar Propostas Orçamentárias';
if ($_REQUEST['acao']== 'C') $titulo_modulo='Consultar Propostas Orçamentárias';
monta_titulo($titulo_modulo,'');
?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form method="POST" name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value="">
<input type=hidden name="acaid" value="">
  <tr>
    <td align='right' width="40%" class="SubTituloDireita">Descrição (ou parte da descrição):</td>
    <td><?=campo_texto('descricao','','','',65,14,'','');?></td>
    
  </tr>
  <tr bgcolor="#C0C0C0">
    <td ></td>
    <td><input type='button' class="botao" name='consultar' value='Consultar' onclick="Consultar()"></td>
  </tr>
</form>
</table>
<?
 unset($nlinhas);
 unset($sql);

$cabecalho = array('CPF','NOME','EMAIL','ORGÃO','DATA LOGIN','ÚLTIMO ACESSO','TEMPO LOGADO');
if ($_REQUEST['descricao']) $texto.= " and Lower(ppodsc) like '%".strtolower($_REQUEST['descricao'])."%'";

    $sql= "select * from seguranca.usuariosonline";
	$db->monta_lista($sql,$cabecalho,100,20,'','','');

?>


<script>
    function Consultar() {
	    document.formulario.act.value = "procura";
	    document.formulario.submit()
    }
    function consultar_proposta(cod){
        document.formulario.act.value = "consultar";
        document.formulario.acaid.value = cod;
	    document.formulario.submit()
    }
    function alterar_proposta(cod){
        document.formulario.act.value = "alterar";
        document.formulario.acaid.value = cod;
	    document.formulario.submit()
    }
    function excluir_proposta(cod){
        document.formulario.act.value = "excluir";
        document.formulario.acaid.value = cod;
	    document.formulario.submit()
    }
</script>

