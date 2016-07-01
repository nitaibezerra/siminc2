<?php
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com)
   Módulo:conMontante.inc
   Finalidade: permitir a consulta do cadastro da proposta por Montante Categoria Econômica e Montante por Coluna
 */
$modulo=$_REQUEST['modulo'] ;
$encontrou = false;
$qtdMontante = 0;
$qtddespesaacao = 0;
$descricao = $_REQUEST['descricao'];

if ($_REQUEST['act']== 'excluir')
{

	$qtddespesaacao=(integer)$db->pegaUm( "select count(dpavalor) as contador from elabrev.despesaacao d inner join elabrev.propostaorcamento p ON p.ppoid = d.ppoid where p.ppoid = ".$_REQUEST['ppoid']."");
	if ($qtddespesaacao > 0)
	{
?>
	<script>
			alert('Não foi possível excluir, pois existem valores distribuídos para as despesas da Ação!'); 
            history.back();
//			location.href='elabrev.php?modulo=principal/propostaorcamentaria/conMontante&acao=A'; 
	</script>
<?	   
	}
	
	
	$qtdMontante=(integer)$db->pegaUm( "select count(mtmvlrlimite) as contador from elabrev.montantematriz mm inner join elabrev.limitefonte LF ON LF.mtmid = mm.mtmid where mm.ppoid = ".$_REQUEST['ppoid']."");
	if ($qtdMontante > 0)
	{
?>
	<script>
			alert('Não foi possível excluir, pois existem valores distribuídos para a Proposta!'); 
            history.back();
//			location.href='elabrev.php?modulo=principal/propostaorcamentaria/conMontante&acao=A'; 
	</script>
<?	   
	}
	else
	{
	$sql2="delete from elabrev.limiteppo where ppoid = ".$_REQUEST['ppoid'];
	$res = $db->executar($sql2);
	$sql2="delete from elabrev.despesaacao where ppoid = ".$_REQUEST['ppoid'];
	$res = $db->executar($sql2);
	$sql3="delete from elabrev.montantematriz where ppoid = ".$_REQUEST['ppoid'];
	$res = $db->executar($sql3);
	$sql2="delete from elabrev.propostaorcamento where ppoid = ".$_REQUEST['ppoid'];
	$res = $db->executar($sql2);
    $db->commit();
?>
	<script>alert('Exclusão efetuada com sucesso!');</script>
<?			
		
	}
}

if ($_REQUEST['act']== 'consultar')
{
   header("Location:elabrev.php?modulo=principal/propostaorcamentaria/montante&acao=A&idproposta=".$_REQUEST['ppoid']);
    exit();
}
if ($_REQUEST['act']== 'alterar')
{
   header("Location:elabrev.php?modulo=principal/propostaorcamentaria/montante&acao=A&idproposta=".$_REQUEST['ppoid']);
    exit();
}
include APPRAIZ."includes/cabecalho.inc";
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
<input type=hidden name="ppoid" value="">
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

$cabecalho = array('Ações','ANO','Descrição','Status','Total');
if ($_REQUEST['descricao']) $texto.= " and Lower(ppodsc) like '%".strtolower($_REQUEST['descricao'])."%'";
/*
    $sql=
    	" select
    		distinct
    			'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Proposta \" onclick=\"alterar_proposta(' || a.ppoid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Proposta \" onclick=\"excluir_proposta(' || a.ppoid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/consultar.gif\" title=\" Administrar tipo \" onclick=\"administrar_tipo(' || a.ppoid || ')\">',
    		a.ppoano,
    		a.ppodsc,
    		Case a.ppostatus when 'H' then 'Hipótese' when 'A' then 'Ativo' end as Status,
    		replace( to_char( SUM ( b.lmpvalor ), '999,999,999,999' ), ',','.' ) as valor
    	from elabrev.propostaorcamento a
    		left join elabrev.limiteppo b on b.ppoid = a.ppoid
    	where ppoanoexercicio = " . $_SESSION['exercicio'] . $texto . "
    group by a.ppoid, a.ppoano, a.ppodsc, a.ppostatus order by a.ppoano";
   */
        $arPerfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
        $sql =
    	" select distinct '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Proposta \" onclick=\"alterar_proposta(' || a.ppoid || ')\">'";
        if (in_array(PERFIL_ADMINISTRADOR_PROPOSTAS, $arPerfis)) {
            $sql .= " || '&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Proposta \" onclick=\"excluir_proposta(' || a.ppoid || ')\">'";
        }
        $sql .= ", a.ppoano, a.ppodsc,
    		Case a.ppostatus when 'H' then 'Hipótese' when 'A' then 'Ativo' end as Status,
    		replace( to_char( SUM ( b.lmpvalor ), '999,999,999,999' ), ',','.' ) as valor
    	from elabrev.propostaorcamento a
    		left join elabrev.limiteppo b on b.ppoid = a.ppoid
    	where tppid=1 and ppoanoexercicio = '" . $_SESSION['exercicio'] . "' " . $texto . "
    group by a.ppoid, a.ppoano, a.ppodsc, a.ppostatus order by a.ppoano";
	$db->monta_lista($sql,$cabecalho,100,20,'','','');

?>


<script>
    function Consultar() {
	    document.formulario.act.value = "procura";
	    document.formulario.submit()
    }
    function consultar_proposta(cod){
        document.formulario.act.value = "consultar";
        document.formulario.ppoid.value = cod;
	    document.formulario.submit()
    }
    function alterar_proposta(cod){
        document.formulario.act.value = "alterar";
        document.formulario.ppoid.value = cod;
	    document.formulario.submit()
    }
    <?php if (in_array(PERFIL_ADMINISTRADOR_PROPOSTAS, $arPerfis)): ?>
    function excluir_proposta(cod){
        document.formulario.act.value = "excluir";
        document.formulario.ppoid.value = cod;
	    document.formulario.submit()
    }
    <?php endif; ?>
    function administrar_tipo(cod){
    	window.location.href = 'elabrev.php?modulo=principal/tipodetalhamento&acao=A&ppoid=' + cod;
    }
</script>

