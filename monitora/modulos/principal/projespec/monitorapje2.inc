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
 
     $sql= "insert into monitora.avaliacaogenerico (avgrealizado,pjeid,usucpf,corcod,tpscod,avgtexto,avgdata) values (".$_REQUEST['avgrealizado'].",".$_SESSION['pjeid'].",'".$_SESSION['usucpf']."',".$_REQUEST['corcod'].",".$_REQUEST['tpscod'].",'".$_REQUEST['avgtexto']."','".$_REQUEST['avgdata']."')";
      $db->executar($sql);
      $db->commit();
      $db->sucesso($modulo);
}

if ($_REQUEST['act']== 'alterar')
{
 
     $sql= "update monitora.avaliacaogenerico set ".
     " avgrealizado=".$_REQUEST['avgrealizado'].
     ",pjeid=".$_SESSION['pjeid'].
     ",usucpf='".$_SESSION['usucpf'].
     "',corcod=".$_REQUEST['corcod'].
     ",tpscod=".$_REQUEST['tpscod'].
     "',avgtexto='".$_REQUEST['avgtexto'].
     ",avgdata='".$_REQUEST['avgdata']."'";

      $db->commit();
      $db->sucesso($modulo);
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
$sql= "select p.* , m.unmdsc, pr.prodsc from monitora.projetoespecial p inner join unidademedida m on p.unmcod=m.unmcod inner join produto pr on pr.procod=p.procod where p.pjeid=".$_SESSION['pjeid'];

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$res =  $db->carrega_registro($RS,0);
// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

?>
<form name="formulario" method="post">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<input type=hidden name="doccod" >
<input type=hidden name="tipograva">
<input type=hidden name="procura">
<input type=hidden name="avptexto" value=0>

<center>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce_gzip.php"></script>
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
<?
   $sql = "select distinct pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra, usu.usunome || ' ('|| org.orgdsc ||')' as usuario from perfil pfl left join monitora.usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.pjeid = ".$_SESSION['pjeid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where  pfl.pflstatus='A' and pfl.pflresponsabilidade in ('E') order by pfl.pfldsc";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
            if ((!$mostra or $mostra=='f') and $usuario ){
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario."</td></tr>";
            print $linha;
            }
        }
     }
     
?>
<tr>
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
?>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">

   	<tr>
        <td align='right' class="SubTituloDireita">Previsto no projeto:</td>
        <td><b>
		<?=number_format($pjeprevistoano,0,',','.');?>
	</b></td>
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
	    <?  $sql = "select tpscod as CODIGO,tpsdsc as descricao from tiposituacao where tpsstatus='A' order by tpsdsc ";
		    $db->monta_combo("tpscod",$sql,'S',"Escolha uma situação",'','','Informe a situação.');?>
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
    <? if (($autoriza or $coordpje or $digit) and ($_REQUEST['act']=='alterar')) {?>
    <tr bgcolor="#C0C0C0"><td></td>
       <td colspan=2><input type="button" class="botao" name="btgravar" value="Alterar" onclick="validar_cadastro(<?=$_REQUEST['doccod']?>)"><input type="reset" class="botao" value="Voltar" onclick="history.back()"></td>
    </tr>
    <?} else if ($autoriza or $coordpje or $digit) {?>
    <tr bgcolor="#C0C0C0"><td></td>
       <td colspan=2><input type="button" class="botao" name="btgravar" value="Incluir" onclick="validar_cadastro('0')"></td>
    </tr>
    <?}?>
</table>

</form>
<?

//teste utilizando a função Monta Lista
$cabecalho = array('Ações','Autor','Data','Situação');

$sql= "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar avaliação\" onclick=\"altera_aval('||a.avgid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir avaliação\" onclick=\"exclui_aval('||a.avgid||')\">' as acoes, u.usunome,to_char(a.avgdata,'dd/mm/yyyy'),'<img border=\"0\" src=\"../imagens/'||c.corimgav||'\" title=\"'||c.corsignificado||'\">' as situacao from monitora.avaliacaogenerico a inner join seguranca.usuario u on u.usucpf=a.usucpf inner join cor c on c.corcod=a.corcod where a.pjeid=".$_SESSION['pjeid']."  order by a.avgdata desc,u.usunome";
//'<img border=\"0\" src=\"../imagens/'||c.corimgav||'\" title=\"'||c.corsignificado||'\" >'
$db->monta_lista($sql,$cabecalho,100,20,'','','');
?>


<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
<script>
    function verregistro(cod) {
        document.formulario.navega.value = cod;
	document.formulario.submit();
    }
   
    function grava_av(refcod,cod,i)
    {
      document.formulario.refren.value=refcod;
      document.formulario.tipograva.value='A';
      document.formulario.doccod.value=cod;
      document.formulario.avptexto.value = i;
      if (submited)
      {
      	alert ('Aguarde o Processamento!');
      }
      else
      {
	      document.formulario.submit();
	      var submited = true;
      }
    }
    function libera_av(refcod,cod,i)
    {

      if (document.formulario.refcod.value=='x' && cod =='0')
      {
         if( window.confirm( "Você optou por liberar as avaliações em todos os períodos.\n Caso queira liberar apenas uma avaliação, cancele sua escolha e\n opte por uma referência específica.\n\n Confirma sua escolha?") )
         {
            document.formulario.refren.value=refcod;
            document.formulario.tipograva.value='LA';
            document.formulario.doccod.value=cod;
            document.formulario.avptexto.value = i;
            document.formulario.submit();
         }
         else document.formulario.exclui.value = 0;
      } else
      {
            document.formulario.refren.value=refcod;
            document.formulario.tipograva.value='LA';
            document.formulario.doccod.value=cod;
            document.formulario.avptexto.value = i;
            document.formulario.submit();
      }
    
    
      
    }
    function bloquea_av(refcod,cod,i)
    {
      document.formulario.refren.value=refcod;
      document.formulario.tipograva.value='BA';
      document.formulario.doccod.value=cod;
      document.formulario.avptexto.value = i;
      document.formulario.submit();
    }
	function visualiza(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/mostraavpar&acao=A&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=480'");
	}
    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
    function Escolhe_referencia()
	{
      	document.formulario.submit();
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


</script>
