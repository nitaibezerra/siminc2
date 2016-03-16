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
$status = '';
$ano = '';

$tpdid = $_REQUEST['tpdid'];
 
 
if ($_REQUEST['acao_n']=='inclui' or $_REQUEST['acao_n']=='altera')
{
	
	// mandou gravar
	$tpdcod = $_REQUEST['tpdcod'];
	$descricao = $_REQUEST['tpddsc'];
	$status = $_REQUEST['tpdstatus'];
	$ano = $_REQUEST['tpdano'];
		
    if ($_REQUEST['acao_n']=='altera') 
    { 
    	$sql="update elabrev.tipodetalhamento set tpdcod='$tpdcod', tpddsc='$descricao', tpdstatus='$status', tpdano = '$ano' where tpdid=".$tpdid." ";
    }
    else 
    {
    	
    	$sql="insert into elabrev.tipodetalhamento (tpdcod, tpddsc, tpdstatus, tpdano) values ('$tpdcod','$descricao','$status','$ano')";
    }

    $res = $db->executar($sql);
    $db->commit();
   $db->sucesso($modulo);    		

}

if ($_REQUEST['acao_n']=='excluir')
{
	$qtd = 0;
	$qtd=(integer)$db->pegaUm( "Select COUNT(*) from elabrev.tipodetalhamentoacao where tpdid=".$tpdid."");
	if ($qtd == 0)
	{
		$sql = "delete from elabrev.tipodetalhamento where tpdid=".$tpdid."";
	//    dbg($sql,1);
	    $saida = $db->executar($sql);
	    $db->commit();
	    $db->sucesso($modulo);    
	}
	else 
	{
?>
		<script>
		alert('Não foi possível excluir. Existem registros vinculados.');
		history.back();
		</script>		
<?	}
}
 

if ($_REQUEST['acao_n']=='exibir')
{
    $sql = "select * from elabrev.tipodetalhamento where tpdid=".$tpdid."";
	$saida = $db->recuperar($sql);
     if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}

}

include  APPRAIZ."includes/cabecalho.inc";
print "<br>";
//$db->cria_aba($abacod_tela,$url,'');
monta_titulo($titulo_modulo,null);

?>
<div align="center">
<form method="POST"  name="formulario">
<input type=hidden name="acao_n">
<input type=hidden name="tpdid" value=<?=$_REQUEST['tpdid']?>>
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<input type=hidden name="tpdstatus" value="A">
    <center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td colspan="2" class="SubTituloDireita" ></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita" >Código:</td>
        <td >
		<?=campo_texto('tpdcod','S','','',4,2,'##','');?></td>
      </tr>      <tr>
        <td align='right' class="SubTituloDireita" >Descrição:</td>
        <td >
		<?=campo_texto('tpddsc','S','','',60,100,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita" >Ano:</td>
        <td >
		<?=campo_texto('tpdano','S',$ordem,'',6,4,'####','');?></td>
      </tr>
     <tr bgcolor="#C0C0C0">       
<?   if (! $_REQUEST['tpdid']){?>
      
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
$cabecalho = array('Ações', 'Código', 'Ano', 'Descrição');
$sql= "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Tipo Detalhamento\" onclick=\"altera_tipodetalhamento('||a.tpdid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Tipo Detalhamento\" onclick=\"excluir_tipodetalhamento('||a.tpdid||')\">' as acao, a.tpdcod, a.tpdano, a.tpddsc from elabrev.tipodetalhamento a order by a.tpdcod, a.tpdano, a.tpddsc ";
$db->monta_lista($sql,$cabecalho,200,20,'','','');
?>


<script>
 
   function gravar_janela(cod)
   {


   	if (!validaBranco(document.formulario.tpdcod, 'Código')) return;	 

   	if (!validaBranco(document.formulario.tpddsc, 'Descrição')) return;	 
		  	
	if (!validaBranco(document.formulario.tpdano, 'Ano')) return;	 

	if (cod=='I') document.formulario.acao_n.value='inclui';
   	   	else document.formulario.acao_n.value='altera';
   	   	// checar consistencias

   	   	
   	   	document.formulario.submit();
   }
   
   
  function altera_tipodetalhamento(cod) { 
	document.formulario.acao_n.value = 'exibir';
	document.formulario.tpdid.value = cod;
	document.formulario.submit();
  }   


  function excluir_tipodetalhamento(cod) { 
    if( window.confirm( "Confirma a exclusão do ítem "+ cod + " no Tipo Detalhamento?") )
    {
	document.formulario.acao_n.value = 'excluir';
	document.formulario.tpdid.value = cod;
	document.formulario.submit();
    } else return;
  }   

</script>
