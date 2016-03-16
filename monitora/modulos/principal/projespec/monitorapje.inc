<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:monitorapje.inc
   finalidade: permitir a avaliação do projeto especial
   */
$modulo=$_REQUEST['modulo'];
if (! $_SESSION['pjeid']) 
{
	?>
	<script>alert('Escolha primeiro o Projeto Especial!');
	history.back();</script>
	<?
	$db->close();
	exit();
}

if ($_REQUEST['act']== 'inserir')
{
 
     $sql= "insert into monitora.avaliacaogenerico (avgrealizado,pjeid,usucpf,tpscod,avgtexto,avgdata,corcod) values (".$_REQUEST['avgrealizado'].",".$_SESSION['pjeid'].",'".$_SESSION['usucpf']."',".$_REQUEST['tpscod'].",'".$_REQUEST['avgtexto']."','".$_REQUEST['avgdata']."',".$_REQUEST['corcod'].")";

     $db->executar($sql);
     $sql = "update monitora.projetoespecial set tpscod=".$_REQUEST['tpscod']." where pjeid=".$_SESSION['pjeid'];
      $db->executar($sql);
      $db->commit();
      $db->sucesso($modulo);
}

if ($_REQUEST['act']== 'alterar')
{
 
     $sql= "update monitora.avaliacaogenerico set ".
     " avgrealizado=".$_REQUEST['avgrealizado'].
      ",usucpf='".$_SESSION['usucpf'].
      "',tpscod=".$_REQUEST['tpscod'].
      ",corcod=".$_REQUEST['corcod'].
     ",avgtexto='".$_REQUEST['avgtexto'].
     "',avgdata='".$_REQUEST['avgdata']."' where avgid=".$_REQUEST['doccod'];
     $db->executar($sql);
     $sql = "update monitora.projetoespecial set tpscod=".$_REQUEST['tpscod']." where pjeid=".$_SESSION['pjeid'];
      $db->executar($sql);     
      $db->commit();
      //$db->sucesso($modulo);
}


if ($_REQUEST['act']== 'bloquear')
{
     $sql= "update monitora.avaliacaogenerico set avgliberada='f' where avgid=".$_REQUEST['doccod'];
	 $db->executar($sql);
	 $db->commit();
	 $db->sucesso($modulo);
}

if ($_REQUEST['act']== 'liberar')
{
     $sql= "update monitora.avaliacaogenerico set avgliberada='t' where avgid=".$_REQUEST['doccod'];
	 $db->executar($sql);
	 $db->commit();
	 $db->sucesso($modulo);
}

if ($_POST['exclui'])
{ 
    $sql = "update monitora.avaliacaogenerico set avgstatus='I' where avgid=".$_REQUEST['doccod'];
    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);
}

  $autoriza = false;
  $coordpje=false;
  $digit=false;
  if ($db->testa_responsavel_projespec($_SESSION['pjeid'])) {
  	$coordpje = true;
  }
   // verifica se é digitador
  if ($db->testa_digitador($_SESSION['pjeid'],'E')) {
  	$digit = true;
  }
// verific se é super-usuário
  if ($db->testa_superuser())  {
  	$coordpje = true; 
  }

include  APPRAIZ."includes/cabecalho.inc";
print '<br>';
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Avaliação de Projeto Especial';
monta_titulo($titulo_modulo,'');
$sql= "select p.* , m.unmdsc, pr.prodsc from monitora.projetoespecial p inner join unidademedida m on p.unmcod=m.unmcod inner join produto pr on pr.procod=p.procod  where p.pjeid=".$_SESSION['pjeid'];
$res =  $db->pegalinha($sql);

// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

/*
u.usucpf,u.usuemail,u.usunome,u.usufoneddd||'-'||u.usufonenum as telefone, un.unidsc

inner join seguranca.usuario u on u.usucpf=p.usucpf inner join unidade un on un.unicod=u.unicod
*/

?>
<form name="formulario" method="post">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<input type=hidden name="doccod" >
<input type=hidden name="tipograva">
<input type=hidden name="exclui">
<input type=hidden name="avptexto" value=0>

<center>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	mode : "textareas",
	theme : "advanced",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, print,preview,fullscreen,separator",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	entity_encoding : "raw",
	width : "100%"
	});
</script>
<!--<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">-->
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita">Denominação:</td>
        <td><?=$pjecod.'.'.$pjedsc?></td>
      </tr>
	<tr>
		<td class="SubTituloDireita">Arquivos vinculados ao projeto:</td>
		<td>
			<? 
				$sql=sql_vincula_arquivo('monitora.projetoespecial',$_SESSION[ 'pjeid' ],'pjeid');
				$insere=0; 
				if ($coordpje or $digitador) $insere=1;		   
     			popup_arquivo( 'arquivo_pje', $sql, 'pjeid',$_SESSION[ 'pjeid' ], $insere, 400, 400,'monitora.projetoespecial' );
			?>
		</td>
	</tr>      
  <tr>
  <td></td>
  <td>
  <input type="button" name="btequipe" value="Equipe do Projeto" onclick="mostra_equipe()">
  </td>
  </tr>	
     

</table>
<?
if ($_REQUEST['act']=='alt' )
{
	// então altera
	$sql = "select a.*, u.usucpf||'-'||u.usunome from monitora.avaliacaogenerico a inner join seguranca.usuario u on u.usucpf=a.usucpf where avgid=".$_REQUEST['doccod'];
	$res = $db->pegalinha($sql);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 

	
}
else {
	print '';
}
/*
select p.* , m.unmdsc, pr.prodsc, u.usucpf,u.usuemail,u.usunome,u.usufoneddd||'-'||u.usufonenum as telefone, un.unidsc from monitora.projetoespecial p inner join unidademedida m on p.unmcod=m.unmcod inner join produto pr on pr.procod=p.procod inner join seguranca.usuario u on u.usucpf=p.usucpf inner join unidade un on un.unicod=u.unicod where p.pjeid=".$_SESSION['pjeid'];
*/
$sql = "select avgrealizado from monitora.avaliacaogenerico where pjeid=".$_SESSION['pjeid']." order by avgdata desc limit 1";
$realizado = $db->pegaUm($sql);
$percent='0';
if ($pjeprevistoano) 
   $percent = number_format(($realizado/$pjeprevistoano)*100,0,',','.');

?>
<br>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
   	<tr>
        <td align='right' class="SubTituloDireita">Produto:</td>
        <td><b><?=$prodsc?></b></td>
    </tr> 
   	<tr>
        <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
        <td><b><?=$unmdsc?></b></td>
    </tr>
   	<tr>
        <td align='right' class="SubTituloDireita">Meta:</td>
        <td><b><?=number_format($pjeprevistoano,0,',','.');?></b></td>
    </tr>     
    <tr>
        <td align='right' class="SubTituloDireita">Realizado até o momento / (Percentual):</td>
        <td><b><?=$realizado.' / ('.$percent.'%)'?></b></td>
    </tr> 
     
  <?      
	 $sql="	select p.pjevlrano as vlrano,sum(e.expfinanceiro) as gasto
          from monitora.projetoespecial p
           left join monitora.execucaopto e on p.pjeid=e.pjeid
         where p.pjeid=".$_SESSION['pjeid']." group by p.pjevlrano";

       $res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 
       $financeiro="<b>Total do projeto: R$ ".number_format($vlrano,2,',','.').
                   "<b><br>Gasto: R$ ".number_format($gasto,2,',','.').
                   "<b><br>Saldo: R$ ".number_format($vlrano-$gasto,2,',','.');             
                   ?>
<tr><td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Financeiro:</td><td> <?=$financeiro?>                  
         </td></tr>
   	<tr>
        <td align='right' class="SubTituloDireita">Data:</td>
        <td>
		<?=campo_data('avgdata', 'S','S','','S');?>
	</td>
      </tr>          
         <tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Realizado<br>Físico:</td><td><?=campo_texto('avgrealizado','S',$habil,'',10,8,'###########','');?></td>
      </tr>
      <tr><td align="right" class="subtitulodireita">Situação:</td><td>
	    <?  
	    // com a cláusula abaixo evita-se que algume da equipe de apoio cancele, conclua ou paralise o projeto
	    if ($digit)    $sql = "select tpscod as CODIGO,tpsdsc as descricao from tiposituacao where tpscod in (1,4,6,8,9) and tpsstatus='A' order by tpsdsc ";
	    else $sql = "select tpscod as CODIGO,tpsdsc as descricao from tiposituacao where tpsstatus='A' order by tpsdsc ";

		    $db->monta_combo("tpscod",$sql,'S',"Escolha uma situação",'','','Informe a situação. Tenha atenção que situações extremas como Conclusão e Cancelamento vão parar o projeto!');?>
		  </td></tr>
		 <tr><td align="right" class="subtitulodireita">Cor:</td><td>
		  <? $sql = "select corcod as codigo,corsignificado as desc , corimgav as imagem from cor where corstatus='A' and corcod>0 order by corcod ";
		  $RSradio = $db->record_set($sql);
	      $nlinhasradio = $db->conta_linhas($RSradio);
	      for ($j=0; $j<=$nlinhasradio;$j++)
	      {
	        $res2 =  $db->carrega_registro($RSradio,$j);
	        if(is_array($res2)) foreach($res2 as $k=>$v) ${$k}=$v;
	        if ($codigo == $corcod) $ck = ' checked '; else $ck ='';
	        print "<input type='radio' value=".$codigo.$ck." name='corcod'  title='".$desc."'>";
	        print "<img src='../imagens/".trim($imagem)."' title='".$desc."'>";
          }
          ?>
          </td></tr>
          <tr>
        <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Observações:</td><td><?=campo_textarea("avgtexto",'N',"S",'',55,7,'','','','Escreva a observação que você considera importante para ser visualizada pela gerência do projeto.')?></td>
    </tr>
    <? if (($coordpje or $digit) and ($_REQUEST['act']=='alt')) {?>
    <tr bgcolor="#C0C0C0"><td></td>
       <td colspan=2>
       <? if (! $avgliberada or $avgliberada=='f') {?>
       <input type="button" class="botao" name="btgravar" value="Alterar" onclick="validar_cadastro(<?=$_REQUEST['doccod']?>)">
       <?}?>
       
       <input type="reset" class="botao" value="Voltar" onclick="history.back()">

    <?} else if ($coordpje or $digit) {?>
    <tr bgcolor="#C0C0C0"><td></td>
       <td colspan=2><input type="button" class="botao" name="btgravar" value="Incluir" onclick="validar_cadastro('0')">

    <?}    
    if ($coordpje)
      {
         if ((! $avgliberada or $avgliberada=='f') and $avgid > 0)
         {
            ?> &nbsp;&nbsp;&nbsp;<input type="button" class="botao" name="btlibera" value= "Liberar" onclick="libera_av('<?=$avgid?>')">&nbsp;&nbsp;&nbsp;<?
         }
         else if ($avgid > 0)
         {
           ?>&nbsp;&nbsp;&nbsp;<input type="button" class="botao" name="btlibera" value= "Bloquear avaliação liberada" onclick="bloquea_av('<?=$avgid?>')">&nbsp;&nbsp;&nbsp;
           <?
         }
      }
      ?>
      </td></tr>
</table>

</form>
<br>
<form>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="0" class="listagem">
<thead>
    <tr>
      <td class="title"  ><strong>Ações</strong></td>
      <td class="title" ><strong>Autor</strong></td>
      <td class="title" ><strong>Data </strong></td>
      <td class="title" ><strong>Situação</strong></td>
   </tr>
</thead>
<tbody>
<?

$sql= "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar avaliação\" onclick=\"altera_aval('||a.avgid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir avaliação\" onclick=\"exclui_aval('||a.avgid||')\">' as acoes, u.usunome,to_char(a.avgdata,'dd/mm/yyyy') as data, a.avgtexto, a.avgrealizado, s.tpscor as cor ,s.tpsdsc as situacao from monitora.avaliacaogenerico a inner join tiposituacao s on s.tpscod=a.tpscod inner join seguranca.usuario u on u.usucpf=a.usucpf inner join cor c on c.corcod=a.corcod where a.pjeid=".$_SESSION['pjeid']." and avgstatus='A'  order by a.avgdata desc,u.usunome";

  $rs = @$db->carregar( $sql );
  if (  $rs && count($rs) > 0 )
  {  	
	foreach ( $rs as $linha )
  	{  		
		foreach($linha as $k=>$v) ${$k}=$v;	
    	if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
    	
    	$texto = 'Realizado='.$avgrealizado.'<br>'.$avgtexto;
    	
    	$situacao = '<font color="'. $cor . '">' . $situacao . '</font>';
	    $situacao = '<span onmouseover="return escape(\'' . $texto .'\')">' . $situacao . '</span>';
    	
    	?>
    	<tr>
     		<td class="title"><?=$acoes?></td>
      		<td class="title" ><?=$usunome?></td>
      		<td class="title" ><?=$data?></td>
      		<td class="title" ><?=$situacao?></td>
   		</tr>
    	<?
    	
  	}
  }
  ?>
  </tbody>
</table>
</form>

<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
<script>
    function libera_av(cod)
    {

      if( window.confirm( "Você optou por liberar a avaliação.\n Isto significa que ela ficará visível aos usuários que podem consultar o Projeto. \n Confirma sua escolha?") )
         {
            document.formulario.act.value='liberar';
            document.formulario.doccod.value=cod;
            document.formulario.submit();
         }
         else document.formulario.exclui.value = 0;
    }
    
    function bloquea_av(cod)
    {
      if( window.confirm( "Você optou por bloquear a avaliação.\n Isto significa que ela deixará de ser visível aos usuários que podem consultar o Projeto. \n Confirma sua escolha?") )
         {
            document.formulario.act.value='bloquear';
            document.formulario.doccod.value=cod;
            document.formulario.submit();
         }
         else document.formulario.exclui.value = 0;
    }


    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }

 function altera_aval(cod)
 {
 	document.formulario.doccod.value=cod;
    document.formulario.act.value = 'alt';
    document.formulario.submit();
 	
 }
  function exclui_aval(cod)
 {
 	if( window.confirm( "Você optou por excluir a avaliação.\n Confirma sua escolha?") )
         {
            document.formulario.doccod.value=cod;
            document.formulario.exclui.value = 1;
            document.formulario.submit();
         }
 	
 }
 
 function validar_cadastro(codacao)
 {
 	// valida o cadastro
 	 document.formulario.doccod.value=codacao;
 	 if (codacao==0) document.formulario.act.value = 'inserir';
 	 else document.formulario.act.value = 'alterar';
     document.formulario.submit();
 }
   function mostra_equipe()
  {
  	 e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/verequipe&acao=A";
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=800,height=300'");
  }


</script>
