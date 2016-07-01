<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:respaca.inc
   Finalidade: permitir a construção de relatorio de responsabilidades de ações
    */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['relat']=='r')
{
  // monta relatorio
  $i=0;
  $titul = 'Você buscou os Ações que não tenham: ';
  $tit = 'Relatório de ações sem ';
  $sql1 = "select  distinct '<img border=\"0\" src=\"imagens/alterar.gif\" title=\" Alterar Cadastro \" onclick=\"alterar_cad(' || '\'' || a.acaid || '\',\'' || a.prgid || '\'' || ')\">' as acao, a.prgcod ||'-'|| a.acacod || '. ' || a.unicod ||'.'||a.loccod as codigo ,a.acadsc as descricao ";
  $from = " from acao a ";
  $sql = " inner join programa p on p.prgid=a.prgid and p.prgano='".$_SESSION['exercicio']."'  ";
  $where = " where  a.acastatus='A' and a.acasnrap='f' ";
  if ($_REQUEST['cbbgu'])
  {
    // verifico se a ação é bgu
    $where .=" and a.acasnbgu='t' ";
    $titul = 'Você buscou as Ações BGU que não tenham: ';
    $tit = 'Relatório de ações BGU sem ';
  }
  if ($_REQUEST['cbest'])
  {
    // verifico se a ação é bgu
    $where .=" and a.acasnestrategica ='t' ";
    $titul = 'Você buscou as Estratégicas BGU que não tenham: ';
    $tit = 'Relatório de ações Estratégicas sem ';
  }
  $where .=" and a.acaid  not in (select distinct acaid from usuarioresponsabilidade where (acaid is not null and rpustatus='A') and (";
  foreach($_POST as $k=>$v) {
     if (strpos($k,'fl') > 0) {
         $i++;
         if ($i == 1) $where .= " pflcod=".$v;
         else $where .= "   or pflcod=".$v;
         $sqldsc = 'select pfldsc from perfil where pflcod='.$v;
         $titulo=$db->recuperar($sqldsc);
         $titul = $titul.$titulo['pfldsc'].' ';
         $tit = $tit.': '.$titulo['pfldsc'];
     }
  }
  if ($_REQUEST['prgid'])
  {
    $ok=1;
    $where .= " and a.prgid=".$_REQUEST['prgid'];
    $sql .= " and p.prgid=".$_REQUEST['prgid']."  ";    
    $sqldsc = "select prgdsc from programa where prgid=".$_REQUEST['prgid'];
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Programa:'.$titulo['prgdsc'];
    $tit.= '| Programa:'.$titulo['prgdsc'];
  }
  
  if ($_REQUEST['orgcod'])
  {// escolheu o órgão
    $sql .= " inner join unidade u on u.unicod=a.unicod inner join orgao o2 on u.orgcod=o2.orgcod and u.orgcod='".$_REQUEST['orgcod']."'";
    $sqldsc = "select orgdsc from orgao where orgcod='".$_REQUEST['orgcod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Do (a) '.$titulo['orgdsc'];
    $tit.= '| Do (a) '.$titulo['orgdsc'];
  }
 if ($_REQUEST['exceto'])
  {
    $org = substr($_SESSION['ittorgao'],0,2);
    $ok=1;
    //$org = '26';
    $sql.= " inner join unidade un2 on un2.unicod=a.unicod and substr(un2.orgcod,0, 3) not in ($org) ";
    $where.= " and substr(a.unicod,0, 3) not in ($org) ";
    $titul.= '| Exceto o '.$_SESSION['ittabrev'];
    $tit.= '| Exceto o '.$_SESSION['ittabrev'];
  }
    if ($_REQUEST['unicod'])
  {
    $ok=1;
    $where.= " and  a.unicod='".$_REQUEST['unicod']."'";
    $sqldsc = "select unidsc from unidade where unicod='".$_REQUEST['unicod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Unidade Orçamentária: '.$titulo['unidsc'];
    $tit.= '| Unidade Orçamentária: '.$titulo['unidsc'];
  }

  if ($_REQUEST['ungcod'])
  {
        $ok=1;
    $sql .= " inner join unidade unn on unn.unicod = a.unicod inner join unidadegestora ug on ug.unicod=unn.unicod and ug.ungcod='".$_REQUEST['ungcod']."'";
    $sqldsc = "select ungdsc from unidadegestora where ungcod='".$_REQUEST['ungcod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Unidade Gestora: '.$titulo['ungdsc'];
    $tit.= '| Unidade Gestora: '.$titulo['ungdsc'];
  }
 

  // atenção !!!! na sql acima, o espaço após o ponto é importante para não causar erro!!!!


  $sqlrel = $sql1.$from.$sql.$where." )) order by codigo";

  if ($i == 0) unset($sqlrel);
  
 // print $sqlrel;
}
include "includes/cabecalho.inc";
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Relatório de Responsabilidades em Ações';
monta_titulo($titulo_modulo,'');
?>
<div align="center">
<center>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="relat" >

<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
<tr><td colspan=2 align='center'><b> Ações SEM:</b></td></tr>
<?
  $sql = "select distinct pfl.pflcod as codigo, pfl.pfldsc as descricao,pfl.pflsncumulativo  from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod  and rpu.rpustatus='A' where pfl.pflstatus='A' and pfl.pflresponsabilidade in ('A','X','D') order by pfl.pfldsc";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	        $linha = "<tr><td class='SubTituloDireita' ><b>".$descricao."</b></td><td><input type='checkbox' name='pfl".$codigo."' value='".$codigo."'></td></tr>";
            print $linha;
        }
     }
     ?>
<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">Ações BGU:</td>
    <td><input type='checkbox' name='cbbgu'></td>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">Ações Estratégicas:</td>
    <td><input type='checkbox' name='cbest'></td>
</tr>
<tr> <td align='right' class="SubTituloDireita">Programa:</td>
        <td>	<?$sql = "select prgid as CODIGO,prgcod ||'-'||prgdsc as DESCRICAO from programa order by prgcod ";
      $prgid = $_REQUEST['prgid'];
	  $db->monta_combo("prgid",$sql,'S',"Todos os Programas",'atualizaComboAcao','');
	?></td>
</tr>

      <tr>
        <td align='right' class="SubTituloDireita">Orgão:</td>
        <td >
	<?
   $orgcod =$_REQUEST["orgcod"];
    $sql = "select orgcod as CODIGO,orgdsc as DESCRICAO from orgao order by orgdsc ";
	  $db->monta_combo("orgcod",$sql,$habil,"Todos os Órgãos",'atualizaComboUnidade','');
	?><br> <input type="checkbox" name="exceto" value='1' title='Filtrar todos os órgãos com exceção do MEC'><b>Exceto o <?=$_SESSION['ittabrev']?></b></td>
      </tr>
	<?
	  if ($_REQUEST["orgcod"]) {
   $unicod =$_REQUEST['unicod'];
        ?>
      <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Unidade Orçamentária (UO):</td>
         <td >
         <?

$sql = "select unicod as CODIGO,unicod||' - '||unidsc as DESCRICAO from unidade where unistatus='A' and unitpocod='U' and orgcod ='".$_REQUEST["orgcod"]."' order by unidsc ";
$db->monta_combo("unicod",$sql,'S',"Todas as Unidades",'atualizaComboUnidade','');
?>
	</td>
      </tr>
<?}?>
	  <?
	      $org = substr($_SESSION['ittorgao'],0,2);
	  if ($unicod == $org.'101' and $orgcod==$_SESSION['ittorgao']) {?>
      <tr bgcolor="#F2F2F2">
        <td align = 'right' class="subtitulodireita">Unidade Gestora (UG):</td>
         <td >
	<?
	  $sql = "select ungcod as CODIGO,ungcod||' - '||ungdsc as DESCRICAO from unidadegestora where ungstatus='A' and unitpocod='U' and unicod ='".$unicod."' order by ungdsc ";
	  $db->monta_combo("ungcod",$sql,'S',"Todas as Unidades Gestoras",'','');

	?>
	</td>
      </tr>

	  <?}
	?>
<tr bgcolor="#CCCCCC">
   <td></td>   <td align='center'><input type="button" name="btrelat" value="Gerar Relatório" onclick="validar_relatorio()" class="botao"></td>

</tr>
</table>
</center>
</div>
</form>
<?

if ($sqlrel){
    $RS = $db->record_set($sqlrel);
    $_SESSION['LINHA'] = $db->conta_linhas($RS);

  $_SESSION['title'] = $tit;
  $_SESSION['TIT'][0][0] = 'Código ';
  $_SESSION['TIT'][0][1] = 'Título';
  $_SESSION['data']= date('(h:m) d/m/Y');
  $_SESSION['LIN'] = array();
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '</b></td><td>';
    if ($_SESSION['LINHA'] >=0) print '<input type="button" class="botao" name="btrelat" value ="Gerar PDF" align="left" onclick="imprime_rel()">';
    print '</td></tr></table>';
    $cabecalho = array('Ação','Código ','Título');
    $db->monta_lista($sqlrel,$cabecalho,100,200,'','','');
    //
    if ($_SESSION['LINHA'] >= 0) {
        for ($i=0; $i<=$_SESSION['LINHA'];$i++){
            $res = $db->carrega_registro($RS,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
 	       $_SESSION['LIN'][$i][0] = $codigo;
		   $_SESSION['LIN'][$i][1] = $descricao;
        }
     }
}
?>

<script>
var WindowObjectReference; /* Declaring a global variable
which will store a reference to the new window to be created */
function validar_relatorio()
{
    document.formulario.relat.value='r';
   	document.formulario.submit();
}
function alterar_cad(cod,prg)
{
   	location.href = "simec.php?modulo=principal/acao/cadacao&acao=C&acaid="+cod+"&prgid="+prg;
}
function imprime_rel()
{
    <?
       $browser = get_browser();
       $saiu = false;
       foreach ($browser as $name => $value) {
           if ($value == 'IE') {
           $saiu = true;
          }
       }
       if ($saiu){?>
           location.href = "simec.php?modulo=relatorio/prn_geral1&acao=R";
       <?} else { ?>
           e = "simec.php?modulo=relatorio/prn_geral1&acao=R";
          window.open(e, "relatorio","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");

      <? } ?>
}
function atualizaComboUnidade(cod)
  {
	 if (cod > 0) {document.formulario.submit()};
    }
  function atualizaComboAcao(cod)
  {
	 if (cod > 0) {document.formulario.submit()};
    }
</script>
