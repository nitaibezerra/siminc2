<?


 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:respunid.inc
   Finalidade: permitir a construção de relatorio de responsabilidades em unidades
    */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['relat']=='r')
{
	
	
	
  // monta relatorio
  $i=0;
  $titul = 'Você buscou as Unidade que não tenham: ';
  $tit = 'Relatório de Unidades sem ';
  $sql1 = "select  distinct u.unicod as codigo,u.unidsc as descricao ";
  $from = " from unidade u ";
  $sql = " inner join orgao o on o.orgcod=u.orgcod and o.organo='".$_SESSION['exercicio']."' and o.orgcod= '". CODIGO_ORGAO_SISTEMA. "' ";
  $where = " where  u.unistatus='A' ";


  $where .=" and u.unicod  not in (select distinct unicod from elabrev.usuarioresponsabilidade where (unicod is not null and rpustatus='A') and (";
  foreach($_POST as $k=>$v) {
     if (strpos($k,'fl') > 0) {
         $i++;
         if ($i == 1) $where .= " pflcod=".$v;
         else $where .= "   or pflcod=".$v;
         $sqldsc = 'select pfldsc from seguranca.perfil where pflcod='.$v;
         $titulo=$db->recuperar($sqldsc);
         $titul = $titul.$titulo['pfldsc'].' ';
         $tit = $tit.': '.$titulo['pfldsc'];
     }
  }

  // atenção !!!! na sql acima, o espaço após o ponto é importante para não causar erro!!!!


$sqlrel = $sql1.$from.$sql.$where." )) order by codigo";
 if ($i == 0) unset($sqlrel);
  

}

/*
if ($_REQUEST['ano1'] and $_REQUEST['ano2'])
{
	if ($_REQUEST['pfl1']) 
	{
	    $titul = 'Você buscou as Ações que tinham Coordenador em '.$_REQUEST['ano1']. '  e que deixaram de ter em '.$_REQUEST['ano2'];
        $tit = 'Ações que tinham Coordenador em '.$_REQUEST['ano1']. '  e que deixaram de ter em '.$_REQUEST['ano2'];
	$sql= "select a.acacod, a.prgcod, a.unicod, a.loccod from acao a inner join usuarioresponsabilidade ur on ur.acaid = a.acaid and ur.acaid is not null and ur.pflcod in (1) and rpustatus='A' where a.prgano ='".$_REQUEST['ano1']."'";
	//PRINT $sql;
	$RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
			$sql="select a.acaid from acao a where acaid not in (select acaid from usuarioresponsabilidade ur where ur.acaid is not null and ur.pflcod in (1)) and acacod='$acacod' and prgcod='$prgcod' and unicod='$unicod'  and a.prgano='".$_REQUEST['ano2']."'";
			if ($db->pegaUm($sql)> 0) $composto .= $db->pegaUm($sql).',';
			//print $sql;
		}
		$composto .= '0';
        $sqlrel = "select  distinct '<img border=\"0\" src=\"imagens/alterar.gif\" title=\" Alterar Cadastro \" onclick=\"alterar_cad(' || '\'' || a.acaid || '\',\'' || a.prgid || '\'' || ')\">' as acao, a.prgcod ||'-'|| a.acacod as codigo,a.unicod, a.loccod as local ,a.acadsc as descricao from acao a inner join programa p on p.prgid=a.prgid and p.prgano='".$_SESSION['exercicio']."'  where  a.acastatus='A' and a.acasnrap='f' and a.acaid in ($composto) order by unicod,codigo";
	}
	}
	if ($_REQUEST['pfl8']) 
	{
	    $titul = 'Você buscou as Ações que tinham Equipe de Apoio em '.$_REQUEST['ano1']. '  e que deixaram de ter em '.$_REQUEST['ano2'];
        $tit = 'Ações que tinham Equipe de Apoio em '.$_REQUEST['ano1']. '  e que deixaram de ter em '.$_REQUEST['ano2'];
	$sql= "select a.acacod, a.prgcod, a.unicod, a.loccod from acao a inner join usuarioresponsabilidade ur on ur.acaid = a.acaid and ur.acaid is not null and ur.pflcod in (8) and rpustatus='A' where a.prgano ='".$_REQUEST['ano1']."'";
	//PRINT $sql;
	$RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
			$sql="select a.acaid from acao a where acaid not in (select acaid from usuarioresponsabilidade ur where ur.acaid is not null and ur.pflcod in (8)) and acacod='$acacod' and prgcod='$prgcod' and unicod='$unicod'  and a.prgano='".$_REQUEST['ano2']."'";
			if ($db->pegaUm($sql)> 0) $composto .= $db->pegaUm($sql).',';
			//print $sql;
		}
		$composto .= '0';
        $sqlrel = "select  distinct '<img border=\"0\" src=\"imagens/alterar.gif\" title=\" Alterar Cadastro \" onclick=\"alterar_cad(' || '\'' || a.acaid || '\',\'' || a.prgid || '\'' || ')\">' as acao, a.prgcod ||'-'|| a.acacod as codigo,a.unicod, a.loccod as local ,a.acadsc as descricao from acao a inner join programa p on p.prgid=a.prgid and p.prgano='".$_SESSION['exercicio']."'  where  a.acastatus='A' and a.acasnrap='f' and a.acaid in ($composto) order by unicod,codigo";
	}
	}


}
*/
include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Relatório de Responsabilidades em Unidades';
monta_titulo($titulo_modulo,'');
?>
<div align="center">
<center>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="relat" >

<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
<tr><td colspan=2 align='center'><b> Unidades SEM:</b></td></tr>
<?

  $sql = "select distinct pfl.pflcod as codigo, pfl.pfldsc as descricao,pfl.pflsncumulativo  from seguranca.perfil pfl left join elabrev.usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod  and rpu.rpustatus='A' where pfl.pflstatus='A' and pfl.pflresponsabilidade in ('U') and pfl.sisid=".$_SESSION['sisid']." order by pfl.pfldsc";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	        $linha = "<tr><td class='SubTituloDireita' ><b>".$descricao."</b></td><td><input type='checkbox' name='pfl".$codigo."' value='".$codigo."'";
			if ($_REQUEST['pfl'.$codigo]) $linha.= ' checked '; 
			$linha .="></td></tr>";
            print $linha;
        }
     }
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
  unset($_SESSION['TIT']);
  $_SESSION['title'] = $tit;
  $_SESSION['TIT'][0][0] = 'Código ';
  $_SESSION['TIT'][0][1] = 'Unidade';
  $_SESSION['data']= date('(h:m) d/m/Y');
  $_SESSION['LIN'] = array();
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '</b></td><td>';
    if ($_SESSION['LINHA'] >=0) print '<input type="button" class="botao" name="btrelat" value ="Gerar PDF" align="left" onclick="imprime_rel('.$_SESSION['browser'].')">';
    print '</td></tr></table>';
    $cabecalho = array('Código','Unidade');
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

function imprime_rel(cod)
{
     //Abre popup em branco
   	 janela = window.open('about:blank',"relatorio","menubar=no,location=no,open=yes,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");

   	   //Abre relatório pdf no popup
       janela.location ="<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
 
}

</script>

