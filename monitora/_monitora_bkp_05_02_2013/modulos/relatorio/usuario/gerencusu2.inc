<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:gerencusu.inc
   Finalidade: permitir a construção de relatorio gerenciais de usuarios
    */
$modulo=$_REQUEST['modulo'] ;//
unset($_SESSION['LINHA']);
unset($_SESSION['title']);
unset($_SESSION['TIT']);
unset($_SESSION['data']);
unset($_SESSION['LIN']);
$_SESSION['LIN']= array();
$_SESSION['TIT']=array();

if ($_REQUEST['relat']=='r')
{
  $ok=0;
  $sql1 = "select distinct u.usucpf as codigo,case when u.ungcod <> '' then u.usunome ||' / '|| o.orgdsc ||' / '||un.unidsc ||' / '|| ung.ungdsc ||' - '|| u.usufuncao when u.unicod <> '' then u.usunome ||' / '|| o.orgdsc ||' / '||un.unidsc ||' - '|| u.usufuncao else u.usunome ||' / '|| o.orgdsc ||' - '||u.usufuncao end as descricao, un.unidsc from usuario u ";
  $where = "  where  u.usustatus='A' ";
  
  $sql = ' left join orgao o on o.orgcod = u.orgcod left join unidade un on un.unicod =u.unicod  left join unidadegestora ung on ung.ungcod=u.ungcod ';
  $titul = 'Você buscou os Usuários que: ';
  $tit = 'Relatório de usuários: ';
  if ($_REQUEST['orgcod'])
  {
    $sql .=" left join orgao on o.orgcod=u.orgcod and o.orgcod='".$_REQUEST['orgcod']."'";
    $sqldsc = "select orgdsc from orgao where orgcod='".$_REQUEST['orgcod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| pertençam a(ao) '.$titulo['orgdsc'];
    $tit.= '| do '.$titulo['orgdsc'];
  }
    if ($_REQUEST['unicod'])
  {
    $sql .= " inner join unidade und on und.unicod=u.unicod and und.unicod='".$_REQUEST['unicod']."'";
    $sqldsc = "select unidsc from unidade where unicod='".$_REQUEST['unicod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= ' e '.$titulo['unidsc'];
    $tit.= ' e '.$titulo['unidsc'];
  }
  if ($_REQUEST['ungcod'])
  {
    $sql .= " inner join unidadegestora ung2 on ung2.ungcod=u.ungcod and ung2.ungcod ='".$_REQUEST['ungcod']."'";
    $sqldsc = "select ungdsc from unidadegestora where ungcod='".$_REQUEST['ungcod']."'";
    $titulo=$db->recuperar($sqldsc);
    $titul.= ' e '.$titulo['ungdsc'];
    $tit.= ' e '.$titulo['ungdsc'];
  }
    if ($_REQUEST['usunome'])
  {
    $where .= " and u.usunome ilike '%".$_REQUEST['usunome']."%'";
    $titul.= '| contenham '.$_REQUEST['usunome'].' no Nome';
    $tit.= '| que contenham '.$_REQUEST['usunome'].' no Nome';
  }

    if ($_REQUEST['usucpf'])
  {
    $where .= " and u.usucpf ilike '%".ereg_replace ("[-.]", "", $_REQUEST['usucpf'])."%'";
    $titul.= '| tenha '.$_REQUEST['usucpf']." no CPF";
    $tit.= '| tenha '.$_REQUEST['usucpf'].' no CPF';
  }
    if ($_REQUEST['pflcod'])
  {
    $sql .= " inner join perfilusuario pu on u.usucpf=pu.usucpf and pu.pflcod ='".$_REQUEST['pflcod']."'";
    $sqldsc = "select pfldsc from perfil where pflcod=".$_REQUEST['pflcod'];
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Tenham responsabilidade de  '.$titulo['pfldsc'];
    $tit.= '| Tenham responsabilidade de '.$titulo['pfldsc'];
  }
  if ($_REQUEST['exceto'])
  {
    $org = substr($_SESSION['ittorgao'],0,2);
    $where .= " and substr(u.orgcod,0, 3) not in ($org) ";
    $titul.= '| Exceto o '.$_SESSION['ittabrev'];
    $tit.= '| Exceto o '.$_SESSION['ittabrev'];
  }
if ($_REQUEST['pflcod'] and $_REQUEST['prgid'])
  {
    $sqldsc = "select prgdsc from programa where prgid=".$_REQUEST['prgid'];
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Programa:'.$titulo['prgdsc'];
    $tit.= '| Programa:'.$titulo['prgdsc'];
    $prog='ok';
    if (! $_REQUEST['acaid'])
    {
    // verifico se a responsabilidade está associada a programa
        $sql.=" inner join usuarioresponsabilidade ur on ur.usucpf=u.usucpf and ur.pflcod=".$_REQUEST['pflcod']."  inner join acao a on ur.acaid=a.acaid inner join programa p on p.prgid = a.prgid and p.prgid=".$_REQUEST['prgid'];
    }
    else
    {
    $sqldsc = "select acadsc from acao where acaid=".$_REQUEST['acaid'];
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Ação:'.$titulo['acadsc'];
    $tit.= '| Ação:'.$titulo['acadsc'];
    $acao='ok';
        $sql.=" inner join usuarioresponsabilidade ur on ur.usucpf=u.usucpf and ur.pflcod=".$_REQUEST['pflcod']."  inner join acao a on ur.acaid=a.acaid and a.acaid=".$_REQUEST['acaid']."  inner join programa p on p.prgid = a.prgid and p.prgid=".$_REQUEST['prgid'];
        }
  }

  if (! $_REQUEST['pflcod'] and $_REQUEST['prgid'])
  {
    if ($prog<>'ok'){
      $sqldsc = "select prgdsc from programa where prgid=".$_REQUEST['prgid'];
     $titulo=$db->recuperar($sqldsc);
     $titul.= '| Programa:'.$titulo['prgdsc'];
     $tit.= '| Programa:'.$titulo['prgdsc'];
    }
    if (! $_REQUEST['acaid'])
    {
    // verifico se a responsabilidade está associada a programa
        $sql.=" inner join usuarioresponsabilidade ur on ur.usucpf=u.usucpf inner join acao a on ur.acaid=a.acaid inner join programa p on p.prgid = a.prgid and p.prgid=".$_REQUEST['prgid'];
    }
    else
    {
       if ($acao<>'ok'){
             $sqldsc = "select acadsc from acao where acaid=".$_REQUEST['acaid'];
    $titulo=$db->recuperar($sqldsc);
    $titul.= '| Ação:'.$titulo['acadsc'];
    $tit.= '| Ação:'.$titulo['acadsc'];
         }
        $sql.=" inner join usuarioresponsabilidade ur on ur.usucpf=u.usucpf inner join acao a on ur.acaid=a.acaid and a.acaid=".$_REQUEST['acaid']."  inner join programa p on p.prgid = a.prgid and p.prgid=".$_REQUEST['prgid'];
        }
  }
    if ($_REQUEST['cbbgu'])
    {
    // verifico se a ação é bgu
        $sql.=" inner join usuarioresponsabilidade urr on urr.usucpf=u.usucpf inner join acao ac on urr.acaid=ac.acaid and ac.acasnbgu='".$_REQUEST['cbbgu']."' ";
    if ($_REQUEST['cbbgu']=='t') {
    $titul.= '| Ação BGU';
    $tit.= '| Ação BGU';
  } else
  {
        $titul.= '| Ação NÃO BGU';
    $tit.= '| Ação NÃO BGU';

  }
  }
    if ($_REQUEST['cbest'])
    {
    // verifico se a ação é bgu
        $sql.=" inner join usuarioresponsabilidade urrr on urrr.usucpf=u.usucpf inner join acao acc on urrr.acaid=acc.acaid and acc.acasnestrategica='".$_REQUEST['cbest']."' ";
    if ($_REQUEST['cbest']=='t') {
    $titul.= '| Ação Estratégica';
    $tit.= '| Ação Estratégica';
  } else
  {
    $titul.= '| Ação NÃO Estratégica';
    $tit.= '| Ação NÃO Estratégica';

  }
    }
  $sqlrel=$sql1.$sql.$where.'  order by un.unidsc,codigo';
 // print $sqlrel;
}

include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Relatório Gerencial de Usuários';
monta_titulo($titulo_modulo,'');
?>
<div align="center">
<center>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="relat" >

<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
<tr><td colspan=2 align='center'><b>Escolha abaixo os itens desejados e pressione o botão "Gerar Relatório"</b></td></tr>
<tr> <td align='right' class="SubTituloDireita">Orgão:</td>
        <td><?
   $orgcod =$_REQUEST["orgcod"];
        $sql = "select orgcod as CODIGO,orgdsc as DESCRICAO from orgao order by orgdsc ";
	  $db->monta_combo("orgcod",$sql,$habil,"Todos os Órgãos",'atualizaComboUnidade','');
	?><br> <input type="checkbox" name="exceto" value='1' title='Filtrar todos os órgãos com exceção do MEC'><b>Exceto o <?=$_SESSION['ittabrev']?></b></td>
</tr>
<?	  if ($orgcod) {  ?>
      <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Unidade:</td>
         <td >
         <?
  $unicod =$_REQUEST["unicod"];
	  $sql = "select unicod as CODIGO,unidsc as DESCRICAO from unidade where unistatus='A' and unitpocod='U' and orgcod ='".$_REQUEST["orgcod"]."' order by unidsc ";
	  $db->monta_combo("unicod",$sql,$habil,"Todas as Unidades",'atualizaComboUnidade','');
?>
	</td>
      </tr>
	  <?}?>
	  	  <?
	  if ($unicod == '26101' and $orgcod== CODIGO_ORGAO_SISTEMA) {?>

      <tr bgcolor="#F2F2F2">
        <td align = 'right' class="subtitulodireita">Unidade Gestora:</td>
         <td >
	<?
	  $sql = "select ungcod as CODIGO,ungcod||' - '||ungdsc as DESCRICAO from unidadegestora where ungstatus='A' and unitpocod='U' and unicod ='".$unicod."' order by ungdsc ";
	  $db->monta_combo("ungcod",$sql,'S',"Todas as Unidades Gestoras",'','');
	?>
	</td>
      </tr>
	  <?}?>

<tr><td align='right' class="SubTituloDireita">Nome (ou parte):</td>
    <td><?=campo_texto('usunome','N','S','',100,100,'','');?></td>
</tr>
<tr><td align='right' class="SubTituloDireita">CPF:</td>
    <td><?=campo_texto('usucpf','','','',16,14,'###.###.###-##','');?></td>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">Responsabilidade:</td>
    <td><?$sql = "select pflcod as CODIGO,pfldsc as DESCRICAO from perfil order by pfldsc ";
	  $db->monta_combo("pflcod",$sql,'S',"Todas as responsabilidades",'','');
	?></td>
</tr>
<tr> <td align='right' class="SubTituloDireita">Programa:</td>
        <td>	<?$sql = "select prgid as CODIGO,prgcod ||'-'||prgdsc as DESCRICAO from programa order by prgcod ";
      $prgid = $_REQUEST['prgid'];
	  $db->monta_combo("prgid",$sql,'S',"Todos os Programas",'atualizaComboAcao','');
	?></td>
</tr>
<?
	  if ($_REQUEST["prgid"]) {?>
<tr> <td align='right' class="SubTituloDireita">Ação:</td>
        <td>
        <?

        $sql = "select acaid as CODIGO,acacod ||'.'||unicod||'-'||acadsc as DESCRICAO from acao where acasnrap='f' and prgid=".$prgid." order by acacod ";

	  $db->monta_combo("acaid",$sql,'S',"Todas as ações",'','');
	}?></td>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">BGU:</td>
    <td><input type='radio' value='f' name='cbbgu'>Não<input type='radio' value='t' name='cbbgu'>Sim</td>
</tr>
<tr><td colspan=2><hr></td></tr>
<tr><td align='right' class="SubTituloDireita">Estratégica:</td>
    <td><input type='radio' value='f' name='cbest'>Não<input type='radio' value='t' name='cbest'>Sim</td>
</tr>
<tr bgcolor="#CCCCCC">
   <td></td>   <td align='center'><input type="button" name="btrelat" value="Gerar Relatório" onclick="validar_relatorio()" class="botao"></td>
</tr>
</table>
</center>
</div>
</form>
<?

if ($sqlrel){

     unset($_SESSION['rsdados']);
     $RS = $db->record_set($sqlrel);
    $_SESSION['rsdados'] = $db->carrega_tudo($RS);
    $_SESSION['LINHA'] = $db->conta_linhas($RS);
   $_SESSION['title'] = $tit.    '       (Total='.($_SESSION['LINHA']+1).')';

    $_SESSION['data']= date('(h:m) d/m/Y');
    $_SESSION['LIN'] = array();
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '       (Total='.($_SESSION['LINHA']+1).')</b></td><td>';
    if ($_SESSION['LINHA'] >=0) print '<input type="button" class="botao" name="btrelat" value ="Gerar PDF" align="left" onclick="imprime_rel()">&nbsp;&nbsp;<a href="ar505ptb.exe"><img border="0" width="30" heigth="30" src="../imagens/pdf_adobe.jpg" title=" Você não tem o Adobe Reader? Clique aqui!"></a>';
    print '</td></tr></table>';
   ?>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
   <thead>
    <tr>
      <td valign="top" class="title"><strong>Ação</strong></td>
      <td valign="top" class="title"><strong>CPF</strong></td>
      <td valign="top" class="title"><strong>Nome / Órgão</strong></td>
      <td valign="top" class="title"><strong>Unidade</strong></td>
      <td valign="top" class="title"><strong>Responsabilidades</strong></td>
    </tr>
</thead>
<tbody>
<?
  //
    if ($_SESSION['LINHA'] >= 0) {
        for ($i=0; $i<=$_SESSION['LINHA'];$i++){
            $res = $db->carrega_registro($RS,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
            if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
         print '<tr class="'.$marcado.'"><td><img border="0" src="../imagens/alterar.gif" title="Consultar cadastro." onclick="consulta_cad(\''.$codigo.'\')"></td><td>'.formatar_cpf($codigo)."</td><td>$descricao</td><td>$unidsc</td><td>";
         $sql = "select distinct p.pfldsc || case when u.prgid > 0 then ' -   Programa - ' || pg.prgcod ||'-'|| pg.prgdsc when u.acaid > 0 then ' -   Ação - '|| a.acacod||'.'||a.unicod ||'-'||a.acadsc when u.pjeid is null and u.acaid is null and u.prgid is null and u.saoid is null then '' end as resp from perfilusuario pu inner join perfil p on pu.pflcod = p.pflcod left join usuarioresponsabilidade u on u.pflcod=p.pflcod and u.rpustatus='A' and u.usucpf='$codigo' left join programa pg on pg.prgid=u.prgid left join acao a on a.acaid = u.acaid where pu.usucpf='$codigo'";
        $RS2 = $db->record_set($sql);
        $nlinhas = $db->conta_linhas($RS2);
        for ($j=0;$j<=$nlinhas;$j++)
        {
          $res2 = $db->carrega_registro($RS2,$j);
          if(is_array($res2)) foreach($res2 as $k=>$v) ${$k}=$v;
          print $pfldsc.'   -   '.$resp.'<br>';
 	       $_SESSION['LIN'][$codigo][] = $resp;
        }
		   print '</td></tr>';
        }
     }
     print '</tbody>';
}
?>

<script>
function validar_relatorio()
{
    document.formulario.relat.value='r';
   	document.formulario.submit();
}
function consulta_cad(cod)
{
   	location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/usuario/cadusuario&acao=C&usucpf="+cod;
}
function atualizaComboUnidade(cod)
  {
	 if (cod > 0) {document.formulario.submit()};
    }
     function atualizaComboAcao(cod)
  {
	 if (cod > 0) {document.formulario.submit()};
    }
function imprime_rel()
{
    <?
       $browser = get_browser();
       $saiu = false;
       if (is_array($browser)){
       foreach ($browser as $name => $value) {
           if ($value == 'IE') {
           $saiu = true;
          }
       }}
       if ($saiu){?>
           location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenusu&acao=R";
       <?} else { ?>
           e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenusu&acao=R";
          window.open(e, "relatorio","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");

      <? } ?>
}

</script>
