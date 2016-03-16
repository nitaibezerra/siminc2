<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
   Módulo:grupocoluna.inc
   
    */

$descricao = '';
$ordem = '';
$acaid = $_REQUEST['acaid'];
 
 
if ($_REQUEST['acao_n']=='inclui' or $_REQUEST['acao_n']=='altera')
{
	
	// mandou gravar
	$descricao = $_REQUEST['gpmdsc'];
	$ordem = $_REQUEST['gpmordem'];
		
    if ($_REQUEST['acao_n']=='altera') 
    { 
    	$sql="update elabrev.grupomatriz set gpmdsc='$descricao', gpmordem='$ordem' where gpmid=$acaid ";
    }
    else 
    {
    	
    	$sql="insert into elabrev.grupomatriz (gpmdsc,gpmordem) values ('$descricao',$ordem)";
  	
    }

    $res = $db->executar($sql);
    $db->commit();
   $db->sucesso($modulo);    		

}

if ($_REQUEST['acao_n']=='excluir')
{
	$qtd = 0;
	$qtd=(integer)$db->pegaUm( "Select COUNT(*) from elabrev.matriz where gpmid=".$acaid);
	
	if ($qtd == 0)
	{
		$sql = "delete from elabrev.grupomatriz where gpmid=$acaid";
	//    dbg($sql,1);
	    $saida = $db->executar($sql);
	    $db->commit();
	    $db->sucesso($modulo);    
	}
	else 
	{
?>
		<script>
		alert('Não foi possível excluir. Existem colunas neste grupo.');
		history.back();
		</script>		
<?	}
}
 

if ($_REQUEST['acao_n']=='exibir')
{
    $sql = "select * from elabrev.grupomatriz where gpmid=$acaid";
//    dbg($sql,1);
	$saida = $db->recuperar($sql);
     if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}

}


include  APPRAIZ."includes/cabecalho.inc";
print "<br>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo($titulo_modulo,'(Configuração da Proposta Orçamentária)');

?>
<div align="center">
<form method="POST"  name="formulario">
<input type=hidden name="acao_n">
<input type=hidden name="acaid" value=<?=$_REQUEST['acaid']?>>
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
    <center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td colspan="2" class="SubTituloDireita" ></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita" >Descrição:</td>
        <td >
		<?=campo_texto('gpmdsc','S','','',60,100,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita" >Ordem:</td>
        <td >
		<?=campo_texto('gpmordem','S',$ordem,'',6,2,'##','');?></td>
      </tr>
     <tr bgcolor="#C0C0C0">       
<?   if (! $_REQUEST['acaid']){?>
      
      <tr ><td colspan="2" class="SubTituloDireita" align="center"><input type='button' class="botao" value='Incluir' onclick="gravar_janela('I')"></tr>
      <? } else {?>
      <tr ><td colspan="2" class="SubTituloDireita" align="center"><input type='button' class="botao" value='Alterar' onclick="gravar_janela('A')"></tr>      
	<? } ?>
      </tr>
 </table>
 </center>
 </div>
 <br><br>
  </form>
<?
//teste utilizando a função Monta Lista
$cabecalho = array('Ações', 'Ordem', 'Descrição');
$sql= "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Grupo Matriz\" onclick=\"altera_grupomatriz('||a.gpmid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Grupo Matriz\" onclick=\"excluir_grupomatriz('||a.gpmid||')\">' as acao, gpmordem, a.gpmdsc from elabrev.grupomatriz a order by a.gpmordem ";
$db->monta_lista($sql,$cabecalho,200,20,'','','');
?>


<script>
 
   function gravar_janela(cod)
   {

   	if (!validaBranco(document.formulario.gpmdsc, 'Descrição')) return;	 
		  	
	if (!validaBranco(document.formulario.gpmordem, 'Ordem')) return;	 

	if (cod=='I') document.formulario.acao_n.value='inclui';
   	   	else document.formulario.acao_n.value='altera';
   	   	// checar consistencias

   	   	
   	   	document.formulario.submit();
   }
   
   
  function altera_grupomatriz(cod) { 
	document.formulario.acao_n.value = 'exibir';
	document.formulario.acaid.value = cod;
	document.formulario.submit();
  }   


  function excluir_grupomatriz(cod) { 
    if( window.confirm( "Confirma a exclusão do ítem "+ cod + " no Grupo Matriz?") )
    {
	document.formulario.acao_n.value = 'excluir';
	document.formulario.acaid.value = cod;
	document.formulario.submit();
    } else return;
  }   

</script>
