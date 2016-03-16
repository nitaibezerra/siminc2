<? 

$descricao = '';
$undid = $_REQUEST['undid'];	
 
// tratando a atualização, inclusão e exclusão de programas 
if ($_REQUEST['acao_n']=='inclui' or $_REQUEST['acao_n']=='altera')
{
    $descricao       = $_REQUEST['unddsc'];
    $prgplanointerno = $_REQUEST['undtipo'];

    if ($_REQUEST['acao_n']=='altera') 
    { 
    	$sql = "update cte.unidademedida set
                    unddsc          = '%s',
                    undtipo = '%s'
                where
                    undid           = %d";
    }
    else 
    {
    	
    	$sql = "insert into cte.unidademedida (
                    unddsc,
                    undtipo
                ) values ('%s', '%s') -- %d";
  	
    }

    $res = $db->executar(sprintf($sql, pg_escape_string($descricao),
                                       pg_escape_string($prgplanointerno), $undid));
    $db->commit();
    $db->sucesso($modulo);

}

if ($_REQUEST['acao_n'] == 'excluir'){
	
	$msg  = '';
	$prop = 0;
	$prop=(integer)$db->pegaUm( "select	COUNT(*) from cte.proposicaosubacao where proposicaosubacao.undid = ".$undid);
	
	$subacao = 0;
	$subacao =(integer)$db->pegaUm( "select	COUNT(*) from cte.subacaoindicador where subacaoindicador.undid = ".$undid);
	
	if (($prop == 0) && ($subacao == 0))
	{		
		$sql = "delete from cte.unidademedida where undid = $undid";
	
	    $saida = $db->executar($sql);
	    $db->commit();
	    $db->sucesso($modulo);    
	}
	else {
		if($prop > 0){
			$msg = "\\nExiste uma ou mais proposições de sub-ação associadas a Unidade de Medida.";				
		}
		if($subacao > 0){
			$msg .= "\\nExiste uma ou mais sub-ações associadas a Unidade de Medida.";
			}
		
		?>
		<script>

		alert('Não foi possível excluir pois:'+'<?= $msg ?>'+'\nÉ necessário resolver as pendências acima para poder excluir essa Unidade de Medida.');

		location.href="cte.php?modulo=/sistema/cadunidade&acao=C";

		</script>
		<?php
	}
}	
 

if ($_REQUEST['acao_n']=='exibir')
{
    $sql   = "select * from cte.unidademedida where undid = $undid";
    $saida = $db->recuperar($sql);
    if (is_array($saida)) {
        foreach($saida as $k=>$v) ${$k} = $v;
    }
}


include  APPRAIZ."includes/cabecalho.inc";
print "<br>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo($titulo_modulo,'(Configuração da Unidade de Medida)');

?>
<div align="center">
<form method="POST"  name="formulario">
<input type=hidden name="acao_n">
<input type=hidden name="undid" value=<?=$_REQUEST['undid']?>/>
<input type=hidden name="modulo" value="<?=$modulo?>"/>
<input type=hidden name="act" value="0" />
    <center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td colspan="2" class="SubTituloDireita" ></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita" >Descrição:</td>
        <td >
		<?=campo_texto('unddsc','S','','',60,100,'','');?></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita" >Estadual / Municipal:</td>
        <td >
		<?$db->monta_combo( "undtipo", "select tprsigla as codigo , tprdsc as descricao from cte.tiporesponsabilidade  order by tprdsc", "S", "Selecione", "", "" ,"","","S");?>
      </tr>
      
     <tr bgcolor="#C0C0C0">       
<?   if (! $_REQUEST['undid']){?>
      
      <tr ><td colspan="2" class="SubTituloDireita" align="center"><input type='button' class="botao" value='Incluir' onclick="gravar_programa('I')"></tr>
      <? } else {?>
      <tr ><td colspan="2" class="SubTituloDireita" align="center"><input type='button' class="botao" value='Alterar' onclick="gravar_programa('A')">
      <input type='button' class='botao' value='Cancelar' id='btcancelar' name='btcancelar' onclick='history.back();' /></td>
      </tr>      
	<? } ?>
      </tr>
 </table>
 </center>
 </div>
 <br><br>
  </form>
<?php

$cabecalho = array('Ações', 'Descrição','Estadual / Municipal');
$sql= "select 
		'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Programa\" onclick=\"altera_programa('||p.undid||')\">&nbsp;&nbsp;&nbsp;
		<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Programa\" onclick=\"excluir_programa('||p.undid||')\">' 
		as acao,
		p.unddsc,
		t.tprdsc		 
	 from 
	 	cte.unidademedida p inner join cte.tiporesponsabilidade t on p.undtipo = t.tprsigla
	 order by 
	 	p.unddsc";

$sql1= "select p.unddsc , p.undtipo from cte.unidademedida p order by p.unddsc";

$db->monta_lista($sql,$cabecalho,100,20,'','','');
?>


<script>
 
   function gravar_programa(cod)
   {

   	if (!validaBranco(document.formulario.unddsc, 'Descrição')) return;			  	
   	if (!validaBranco(document.formulario.undtipo, 'Estadual / Municipal')) return;			  	
	 

	if (cod=='I') document.formulario.acao_n.value='inclui';
   	   	else document.formulario.acao_n.value='altera';
   	   	// checar consistencias
   	   	
   	   	document.formulario.submit();
   }
   
   
  function altera_programa(cod) { 
	document.formulario.acao_n.value = 'exibir';
	document.formulario.undid.value = cod;
	document.formulario.submit();
  }   


  function excluir_programa(cod) { 
    if( window.confirm( "Confirma a exclusão do programa?") )
    {
	document.formulario.acao_n.value = 'excluir';
	document.formulario.undid.value = cod;
	document.formulario.submit();
    } else return;
  }   

</script>
