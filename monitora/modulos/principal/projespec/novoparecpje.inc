<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   módulo:novoparecpje.inc
   finalidade: permitir administrar o parecer de alta gestão no projeto especial
   */
$modulo=$_REQUEST['modulo'] ;//

if (! $_SESSION['pjeid']) 
{
	?>
	<script>alert('Escolha primeiro o Projeto Especial!');
	history.back();</script>
	<?
	$db->close();
	exit();
}

// testar se houve solicitação de gravar
if ($_REQUEST['tipograva'])
{
     // é uma gravação sem liberação
     // verifico se o cod é zero, se for, será um insert, senão será update
     if ($_REQUEST['tipograva']=='i')
     {
        $sql= "insert into avaliacaoparecer (pjeid,avptexto,avpstatus,usucpf,avporigem,tpaid) values (".
        $_SESSION['pjeid'].",'".
        $_REQUEST['avptexto']."','A','".
        $_SESSION['usucpf']."',9,3)";
      } else
         $sql= "update avaliacaoparecer set avptexto='".$_REQUEST['avptexto']."' where avpid=".$_REQUEST['tipograva'];


     $db->executar($sql);
     $db->commit();
     // envia-email para o coordenador e gerente do projeto e para o proprio
     $sql="select usunome as gestor,usuemail as emailproprio from seguranca.usuario where usucpf='".$_SESSION['usucpf']."'";
      
          
     $res =  $db->pegalinha($sql);
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

   
       
       // acha o gerente do projeto
       $sql="select usunome as gerente,usuemail as emailgerente, pe.pjecod, pe.pjedsc from seguranca.usuario u inner join monitora.usuarioresponsabilidade ur on ur.usucpf=u.usucpf and ur.pjeid=".$_SESSION['pjeid']. " and ur.pflcod=12 inner join monitora.projetoespecial pe on pe.pjeid=ur.pjeid";
       

              
       $res =  $db->pegalinha($sql);
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       
       
       
       // acha o coordenador do projeto
              $sql="select u.usunome as coordenador,u.usuemail as emailcoord from seguranca.usuario u inner join seguranca.perfilusuario pfu on pfu.pflcod=47 and pfu.usucpf=u.usucpf inner join monitora.usuarioresponsabilidade urp on urp.pjeid=".$_SESSION['pjeid']." and urp.usucpf=u.usucpf and urp.rpustatus='A' ";
       
         
              
              $res =  $db->pegalinha($sql);
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

        $assunto = 'Parecer de Alta Gestão em Projeto Especial';
        $mensagem = 'Foi inserido ou alterado no SIMEC um parecer de alta gestão no Projeto especial '.$pjecod.'-'.$pjedsc.'<br> Por '.$gestor;
        email(str_to_upper($gerente), $emailgerente, $assunto, $mensagem);
        email(str_to_upper($coordenador), $emailcoord, $assunto, $mensagem);        
        email(str_to_upper($gestor), $emailproprio, $assunto, $mensagem);
        $db->close();

   ?>
    <script>
    alert('Operação realizada com sucesso!');
    opener.location.reload();
    
    
    
    self.close();
    </script>
    <?
    exit();
}


?>
<title>Parecer de Alta Gestão</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<br>
<?

$titulo_modulo='Parecer de Alta Gestão no Projeto Especial';
monta_titulo($titulo_modulo,'');

$sql= "select p.* , m.unmdsc, pr.prodsc, u.usucpf,u.usuemail,u.usunome,u.usufoneddd||'-'||u.usufonenum as telefone, un.unidsc from monitora.projetoespecial p inner join unidademedida m on p.unmcod=m.unmcod inner join produto pr on pr.procod=p.procod inner join seguranca.usuario u on u.usucpf=p.usucpf inner join unidade un on un.unicod=u.unicod where p.pjeid=".$_SESSION['pjeid'];
//$res =  $db->pegalinha($sql);
// Transforma em variáveis todos os campos do array
//if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
$sql="select pe.pjesnvisivel as visivel,pe.pjesndatafechada as projfechado,pe.pjecod, pe.pjedsc,to_char(pe.pjedataini,'dd/mm/YYYY') as pjedataini,to_char(pe.pjedatafim,'dd/mm/YYYY') as pjedatafim,p.prodsc,pe.pjeprevistoano,pe.pjevlrano from monitora.projetoespecial pe inner join produto p on p.procod=pe.procod where pjeid=".$_SESSION['pjeid'];
$res=$db->pegalinha($sql);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>

<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<form method="post"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">

<center>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	mode : "textareas",
	theme : "advanced",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright, justifyfull",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	entity_encoding : "raw"
	});
</script>


<?
   // verifica se é alta gestão
  $alta_gestao = false;
  $super=false;
$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod=11 and pu.usucpf ='".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro))
  $alta_gestao = true;

  // testo se é super usuário

$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod=6 and p.sisid=".$_SESSION['sisid']." and pu.usucpf ='".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro))  {$alta_gestao = true;$super=true;}
?>

<div align="center">

<?
if ($_REQUEST['pag'])
  {
    if (! $super)
    $sql = 'select * from avaliacaoparecer where avpid='.$_REQUEST['pag']." and usucpf='".$_SESSION['usucpf']."'";
    else
    $sql = 'select * from avaliacaoparecer where avpid='.$_REQUEST['pag'];
    $saida = $db->recuperar($sql);
    if(is_array($saida))
	foreach($saida as $k=>$v) ${$k}=$v;
  }
?>

<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<? if ($_REQUEST['pag'])
  print '<input type=hidden name="tipograva" value='.$_REQUEST['pag'].'>';
  else print '<input type=hidden name="tipograva" value="i">';
    ?>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
  <tr>
    <td align='right' class="SubTituloDireita">Projeto :</td>
    <td><b><?=$pjecod?>&nbsp;-&nbsp;<?=$pjedsc?></b></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Período do Projeto:</td>
    <td><b><?=$pjedataini?>&nbsp; - &nbsp;<?=$pjedatafim?></b></td>
  </tr>  
  <? if ($pjefimmt or $pjeinimt)
  {?>
      <tr>
    <td align='right' class="SubTituloDireita">Período da Macro-Etapa Agregadora :</td>
    <td><b><?=$pjeinimt?>&nbsp; - &nbsp;<?=$pjefimmt?></b></td>
  </tr>
  <?}?>
  <tr>
    <td align='right' class="SubTituloDireita">Produto :</td>
    <td><b><?=$prodsc?></b></td>
  </tr>
        <tr>
    <td align='right' class="SubTituloDireita">Físico e Financeiro :</td>
    <td><b><?=$pjeprevistoano?>&nbsp;/&nbsp;<?='R$ '.number_format($pjevlrano,2,',','.')?></b></td>
  </tr> 
<?
   $sql = "select distinct pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra,". 
"usu.usunome || ' ('|| org.orgdsc ||')' as usuario from perfil pfl left join monitora.usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.pjeid = ".$_SESSION['pjeid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where  pfl.pflstatus='A' and pfl.pflresponsabilidade in ('E') order by pfl.pfldsc";
     $rs2 = @$db->carregar( $sql );
   //$RS2 = $db->record_set($sql);
     if (  $rs2 && count($rs2) > 0 )
     {  	
  	    $j=0;
	    foreach ( $rs2 as $linha2 )
		{
		 foreach($linha2 as $k=>$v) ${$k}=$v;
                     if ((!$mostra or $mostra=='f') and $usuario ){
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario."</td></tr>";
            print $linha;
            }
            $j++;
		}
     }
   
  ?>
</tr>
</table>

<center>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
  <tr><td colspan=2 width='100%'><?=campo_textarea("avptexto",'N','S','',110,28,'');?></td></tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr bgcolor="#C0C0C0">
   <td align='right'><input type="button" class="botao" name="btgravar" value="Gravar" onclick="validar_cadastro()">&nbsp;&nbsp;&nbsp;&nbsp;<input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>

</tr>
 </table>
 </center>
 </div>

</form>

<script>
  function fechar_janela()
  {
    window.close();
    opener.location.reload();
  }

    function validar_cadastro()
    {

	document.formulario.avptexto.value = tinyMCE.getContent('avptexto');
	if (!validaBranco(document.formulario.avptexto, 'Parecer')) return tinyMCE.execCommand('mceFocus', true, 'avptexto');
       document.formulario.submit();
    }
</script>
