<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:novoprograma.inc
   Finalidade: permitir a inclusão de um novo programa
   */
$modulo=$_REQUEST['modulo'] ;//
			$prgid = (integer) $_REQUEST['prgid'];

function atualiza_unidade($prgid, $unicod )
	{
		global $db;
		$prgid = (integer) $prgid;
		if ( !$prgid )
		{
			return;
		}
		$sql_remocao = "delete from elabrev.unidade_acao where prgid = " . $prgid;
		$db->executar( $sql_remocao );
		//foreach ( (array) $regcods as $regcod )
		//{
			$sql_insercao = "insert into elabrev.unidade_acao ( prgid, unicod,usucpf ) values ( " . $prgid . ", '" . $unicod . "','".$_SESSION['usucpf']."' )";
			$db->executar( $sql_insercao );
	

		//}
	}	
	


/**
 * Transforma a primeira letra de cada letra em maiúsculas, exceto as
 * pertencentes a lista indicada na variável $minuscole.
 *
 * @param string $string
 * @return string
 */
function my_ucwords( $string )
{
	$string = strtolower( $string );	
	$minuscole = array(
		'a', 'à', 'e', 'é', 'o',
		'da', 'de', 'do',
		'com', 'para',
		'mas', 'porém', 'então',
		'até',
		'lá',
		'em', 'no', 'na', 'num', 'nuns', 'numa', 'numas',
		'um', 'uns', 'uma', 'umas'
	);
	$result = '';
	foreach ( split( '[[:space:]]', $string ) as $word )
	{
		if ( in_array( $word, $minuscole ) )
		{
			$result .= ' ' . strtolower( $word );
		}
		else
		{
			$result .= ' ' . ucfirst( $word );
		}
	}
	return ucfirst( substr( $result, 1 ) );
}

if ($_REQUEST['prog_n']=='inclui' or $_REQUEST['prog_n']=='altera')
{
	foreach($_REQUEST as $k=>$v) ${$k}=$v;
	// mandou gravar
	$prgdsc=my_ucwords($prgdsc);
    $tpocod='O';
    $orgcod=$_SESSION['ittorgao'];
    
	if (! $prgvlrprograma) $prgvlrprograma=0;
	if (! $descod) $descod=0;

    if ($_REQUEST['prog_n']=='altera') 
    { 
    	$sql="update elabrev.ppaprograma_proposta set prgdsc='$prgdsc',tprcod='$tprcod',prgmesinicio='$prgmesinicio',prganoinicio='$prganoinicio',prgmestermino='$prgmestermino',prganotermino='$prganotermino',prgdscobjetivo='$prgdscobjetivo',prgdscpublicoalvo='$prgdscpublicoalvo',prgdscjustificativa='$prgdscjustificativa',prgdscestrategia ='$prgdscestrategia', prgvlrprograma=$prgvlrprograma,prgsntemporario='$prgsntemporario',prgobs='$prgobs',prgdscproblema='$prgdscproblema',descod ='$descod' where prgid=$prgid ";

    }
    else 
    {

    	$sql=" select prgid from elabrev.ppaprograma_proposta where prgcod='$prgcod' and prsano='".$_SESSION['exercicio']."'";
    	$prgid=$db->pegaUm($sql);
        $prgcod='XXXX';
    	$sql="insert into elabrev.ppaprograma_proposta (prgcod, prgdsc,tpocod,tprcod,orgcod,prgmesinicio,prganoinicio,prgmestermino,prganotermino,prgdscobjetivo,prgdscpublicoalvo,prgdscjustificativa,prgdscestrategia, prgvlrprograma,prgsntemporario,prgobs,prgdscproblema,descod,prsano,prgstatus,usucpf) values ('$prgcod','$prgdsc','$tpocod','$tprcod','$orgcod','$prgmesinicio','$prganoinicio','$prgmestermino','$prganotermino','$prgdscobjetivo','$prgdscpublicoalvo','$prgdscjustificativa','$prgdscestrategia', $prgvlrprograma,'$prgsntemporario','$prgobs','$prgdscproblema',$descod,'".$_SESSION['exercicio']."','N','".$_SESSION['usucpf']."')";

    }
    $res = $db->executar($sql);
    $db->commit();
    $db->sucesso($modulo);    	
}


if ($_REQUEST['prog_n']=='exclui')
{
    $sql = "delete from elabrev.ppaprograma_proposta where prgid=".$_REQUEST['prgid'];
    $res = $db->executar($sql);

    $db->commit();
    $db->sucesso($modulo);

}
foreach($_REQUEST as $k=>$v) ${$k}=$v;
if ($_REQUEST['prgid'] or $_REQUEST['verprgid'])
{
	if ($_REQUEST['verprgid']) $_REQUEST['prgid']=$_REQUEST['verprgid'];
     $sql = "select a.* from ".$_SESSION['sisdiretorio'].".ppaprograma_proposta a where a.prgid=".$_REQUEST['prgid'];
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

    $_SESSION['prgid']=$_REQUEST['prgid'];
}
//
if ($_REQUEST['clone'])
{
     $sql = "select p.* from ".$_SESSION['sisdiretorio'].".ppaprograma_proposta p where p.prgid=".$_REQUEST['clone'];
	$saida = $db->recuperar($sql,$res);
   if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}
   $msg='Dados obtidos a partir de clonagem!';
}
 

include APPRAIZ."includes/cabecalho.inc";

$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Inclusão de um novo Programa';
monta_titulo($titulo_modulo,'');
// variáveis gerais

?>

<script language="Javascript" src="/includes/livesearch.js"></script>
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="clone" value=0>
<input type=hidden name="registro" value=<?=$_POST['registro'];?>>
<input type=hidden name="prog_n">
<input type=hidden name="verprgid" value=0>
<input type=hidden name="prgid" value=<?=$_REQUEST['prgid']?>>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

      <?if ($_REQUEST['prgid']) {
?>      <!--<tr>
        <td align='right' class="SubTituloDireita">Unidades Orçamentárias relacionadas</td>
        <td><?
             //$sql = "select u.unicod as codigo, u.unicod||'-'||u.unidsc as descricao from unidade u inner join elabrev.unidade_acao ua on ua.unicod=u.unicod and ua.prgid = ".$_REQUEST['prgid']."  where u.unitpocod='U' order by u.unicod ";
            //$db->monta_combo('unicod',$sql,'S','Lista das Unidades relacionadas a esta nova ação','','','',400);
            //print "&nbsp;&nbsp;<img src='../imagens/alterar.gif' align='absmiddle' width='15' height='15' title='Incluir Unidades Orçamentárias' onclick=".'"incluir_unid('.$_REQUEST['prgid'].')">'?></td>      
      </tr>-->
       
      <?} else   if ($db->verifica_momento())
   {?>
      <tr>
        <td align='right' class="SubTituloDireita"></td>
        <td><input type="button" name="btngeraclone" value="Gerar Clone." onclick="geraclone()">&nbsp;&nbsp;<?=$msg?></td>
      </tr>       
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Orgão:</td>
        <td >
	<?
   $orgcod =$_SESSION["ittorgao"];
    $sql = "select orgcod as CODIGO,orgdsc as DESCRICAO from orgao where organo='".$_SESSION['exercicio']."' order by orgdsc ";
	  $db->monta_combo("orgcod",$sql,'N',"Todos os Órgãos",'atualizaComboUnidade','','MEC',400);
	?></td>
      </tr>
       <tr>
	      <td align='right' class="SubTituloDireita">Unidade responsável:</td>
        <td><?
			if (! $_SESSION['uniorc']) 
		$sql = "select unicod as CODIGO,unicod||' - '||unidsc as DESCRICAO from unidade where unistatus='A' and unitpocod='U' and orgcod ='".$orgcod."' order by unidsc ";
		else 
		{
			$sql = "select unicod as CODIGO,unicod||' - '||unidsc as DESCRICAO from unidade where unistatus='A' and unitpocod='U' and orgcod ='".$orgcod."' and unicod in (";
			for ($ii=0;$ii<count($_SESSION['uniorc']);$ii++)
			{
				if ($ii > 0 and $ii<count($_SESSION['uniorc'])-1) $sql .= ",";
				$sql .="'".$_SESSION['uniorc'][$ii]."'";
				//if ($ii == 0 and count($_SESSION['uniorc'])>0) $sql .= ",";
				//if ($ii > 0 and $ii<count($_SESSION['uniorc'])-1) $sql .= ",";
			}
			$sql .=") order by unidsc ";
	  	}
			$db->monta_combo('unicod',$sql,'S','Escolha a Unidade Responsável','','','',400);
?></td>           
       </tr>       
       <tr>
	      <td align='right' class="SubTituloDireita">Tipo:</td>
        <td><?
			$sql= "select tprcod as codigo, tprnome as descricao from tipoprograma where tprstatus='A' order by descricao";
			$db->monta_combo('tprcod',$sql,'S','Escolha o tipo de programa','','','',400);
?></td>           
       </tr>      
      <tr>
        <td align='right' class="SubTituloDireita">Título do programa:</td>
        <td><?=campo_textarea('prgdsc','S','S','',80,3,'','','','');?></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Objetivo:</td>
        <td><?=campo_textarea('prgdscobjetivo','S','S','',80,3,'','','','');?></td>         
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Público alvo:</td>  
        <td><?=campo_textarea('prgdscpublicoalvo','S','S','',80,3,'','','','');?></td>          
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Justificativa:</td>  
        <td><?=campo_textarea('prgdscjustificativa','S','S','',80,3,'','','','');?></td>          
      </tr> 
      <tr>
        <td align='right' class="SubTituloDireita">Estratégia:</td>  
        <td><?=campo_textarea('prgdscestrategia','S','S','',80,3,'','','','');?></td>          
      </tr> 
      <tr>
        <td align='right' class="SubTituloDireita">Problema:</td>  
        <td><?=campo_textarea('prgdscproblema','S','S','',80,3,'','','','');?></td>          
      </tr> 
       <tr>
        <td align='right' class="SubTituloDireita">Desafio:</td>
        <td><?
             $sql= "select descod as codigo, desdsc as descricao from desafio where desstatus='A' order by descricao";
            $db->monta_combo('descod',$sql,'S','Escolha o desafio','','','',400);
?>
	</td> 
      </tr>            
      <tr>
        <td align='right' class="SubTituloDireita">Valor estimado:</td>  
        <td><?=campo_texto('prgvlrprograma','S','S','Entre com o valor em Reais em inteiros',50,15,'##################','');?></td>          
      </tr>      
<tr>
        <td align='right' class="SubTituloDireita">Moldura temporal:</td>
        <td><input type="radio" value="f" name="prgsntemporario" <?if ($prgsntemporario=='f') print '   checked'?>>Contínuo     <input type="radio" value="t" name="prgsntemporario" <?if ($prgsntemporario=='t') print '   checked'?>>Temporário</td>
      </tr>              
    
 <? if ($prgsntemporario=='t'){?>
	<tr>
	   <td align='right' class="SubTituloDireita">Período do Programa<br>(Para programas temporários):</td>
       <td>
          <table border=1>
             <tr>
                <td width='50%'>Início</td>
                <td width='50%'>Término</td>
             </tr>
             <tr>
                <td width='50%'><?=campo_texto('prgmesinicio','N',$habil,'',3,2,'##','N');?>/<?=campo_texto('prganoinicio','N',$habil,'',6,4,'####','N');?></td>
                <td width='50%'><?=campo_texto('prgmestermino','N',$habil,'',3,2,'##','N');?>/<?=campo_texto('prganotermino','N',$habil,'',6,4,'####','N');?></td>
             </tr>
          </table>
       </td>
    </tr> 
    <?}?>                 

     <tr>
        <td align='right' class="SubTituloDireita">Observação:</td>
        <td><?=campo_textarea('prgobs','N','S','',80,3,'','','','','');?></td>
      </tr>
  <?
  if ($db->verifica_momento())
  {

   if (! $_REQUEST['prgid']){?>
      
      <tr ><td colspan="2" align="center"><input type='button' class="botao" value='Incluir' onclick="gravar_janela('I')"></tr>
      <? } else if ($db->ehcriador($_REQUEST['prgid'],'P')){?>
      <tr ><td colspan="2" align="center"><input type='button' class="botao" value='Alterar' onclick="gravar_janela('A')"></tr>      
      <?}
  }?>
    </table>
  
    
</form>  
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
    <td colspan="2" valign="top" align="center" class="title"><strong>Novos programas propostos</strong></td>
    </tr>
    <tr>
      <td valign="top" class="title"><strong>Ações</strong></td>
      <td valign="top" class="title"><strong>Proposta (Programa - Título do programa)</strong></td>     
    </tr>
</thead>
<tbody>
<?
$sql= "select p.prgcod as codigo, p.prgcod||'-'||p.prgdsc as descricao, p.prgcod, p.prgid from elabrev.ppaprograma_proposta p  where p.usucpf='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."'  order by p.prgcod,descricao";

$rs = $db->carregar( $sql );
if (  $rs && count($rs) > 0 )
	{
	 $i=0;
	 foreach ( $rs as $linha )
		{
		 foreach($linha as $k=>$v) ${$k}=$v;
		 if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
       	 print "<tr class='$marcado'>
  <td><img border='0' src='../imagens/alterar.gif' title='Alterar a proposta.' onclick=".'"altera_programa('."'$prgid')".'"'.">";
           if ($db->verifica_momento())
  				print "&nbsp;&nbsp;&nbsp;<img border='0' src='../imagens/excluir.gif' title='Excluir a proposta.' onclick=".'"excluir_programa('."$prgid,'$descricao')".'">';
     print "</td><td>$descricao</td></tr>";
  		 $i++;
		}
	}
else
{
  print '<tr class="'.$marcado.'"><td></td><td>Não há registros de novos programas</td></tr>';
}
?>
</tbody>
    </table>
    

<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
    <td colspan="3" valign="top" align="center" class="title"><strong>Novos programas propostos por outros Gestores</strong></td>
    </tr>
    <tr>
      <td valign="top" class="title"><strong>Ações</strong></td>
      <td valign="top" class="title"><strong>Proposta (Programa - Título do programa)</strong></td>        <td valign="top" class="title"><strong>Proponente</strong></td>
      
    </tr>
</thead>
<tbody>
<?
$sql= "select p.prgcod as codigo, p.prgcod||'-'||p.prgdsc as descricao, p.prgcod, p.prgid, case when p.usucpf !='".$_SESSION['usucpf']."'  then '<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || p.usucpf || '\'' || ')\">&nbsp;'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum ||'-'||un.unidsc else u.usunome||'-'||u.usufoneddd||'-'||u.usufonenum||'-'||un.unidsc end as autor from elabrev.ppaprograma_proposta p  inner join seguranca.usuario u on u.usucpf=p.usucpf inner join unidade un on un.unicod=u.unicod where p.usucpf not in ('".$_SESSION['usucpf']."') and prsano='".$_SESSION['exercicio']."'  order by p.prgcod,descricao";

$rs = $db->carregar( $sql );
if (  $rs && count($rs) > 0 )
	{
	 $i=0;
	 foreach ( $rs as $linha )
		{
		 foreach($linha as $k=>$v) ${$k}=$v;
		 if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
       	 print "<tr class='$marcado'>
  <td><img border='0' src='../imagens/alterar.gif' title='Alterar a proposta.' onclick=".'"ver_programa('."'$prgid')".'"'."></td><td>$descricao</td><td>$autor</td></tr>";
  		 $i++;
		}
	}
else
{
  print '<tr class="'.$marcado.'"><td></td><td>Não há registros de novos programas</td></tr>';
}


?>
</tbody>
    </table>   
    
    

<script>

   function gravar_janela(cod)
   {	
   	  //prepara_formulario();
   	  // testar todos os campos antes de incluir

   	  if (!validaBranco(document.formulario.tprcod, 'Tipo de programa')) return;
   	  
   	  if (!validaBranco(document.formulario.prgdsc, 'Título')) return;
   	  
   	  if (!validaBranco(document.formulario.prgdscobjetivo, 'Objetivo')) return;
   	  if (!validaBranco(document.formulario.prgdscpublicoalvo, 'Público alvo')) return;
   	  
   	  if (!validaBranco(document.formulario.prgdscjustificativa, 'Justificativa')) return;
   	  if (!validaBranco(document.formulario.prgdscestrategia, 'Estratégia')) return;
   	  if (!validaBranco(document.formulario.prgdscproblema, 'Problema')) return;
   	 	  
   	  if ( !validar_radio( document.formulario.prgsntemporario, 'Moldura temporal' ) ) {
			alert ('Moldura temporal!');
			document.formulario.prgsntemporario.focus();
			return;
		} 
	  
/*
   	  if (document.formulario.prgsntemporario[1].checked)     	     	  
   	  { // é temporário
   	  	if (document.formulario.prgmesinicio.value <0 ||  document.formulario.prgmesinicio.value >12)
   	  	{
   	  		alert ('Mês de início inválido!');
			document.formulario.prgmesinicio.focus();
			return;
   	  	}
   	  	if (document.formulario.prgmestermino.value <0 ||  document.formulario.prgmestermino.value >12)
   	  	{
   	  		alert ('Mês de término inválido!');
			document.formulario.prgmestermino.focus();
			return;
   	  	}   	  	

 	  	if (!validaBranco(document.formulario.prgmesinicio, 'Mês de início do programa')) return; 
   	  	if (!validaBranco(document.formulario.prganoinicio, 'Ano de início do programa')) return; 
   	  	if (!validaBranco(document.formulario.prgmestermino, 'Mês de término do programa')) return; 
   	  	if (!validaBranco(document.formulario.prganotermino, 'Ano de término do programa')) return;    
   	  	if (document.formulario.prganotermino.value < document.formulario.prganoinicio.value)	  	{
   	  		alert ('Ano de término do programa não pode ser anterior ao de início');
   	  		return;
   	  	}
   	  	else if (document.formulario.prganotermino.value == document.formulario.prganoinicio.value && document.formulario.prgmestermino.value < document.formulario.prgmesinicio.value)
   	  	{
   	  		alert ('Mês de término do programa não pode ser anterior ao de início');
   	  		document.formulario.acamestermino.focus();
   	  		return;   	  		
   	  	}
   	  	  	
   	  }
*/ 	  
   	  if (cod=='I') document.formulario.prog_n.value='inclui';
   	  else document.formulario.prog_n.value='altera';
   	   	   document.formulario.submit();
   }

  
  function altera_programa(cod) {
     document.formulario.prgid.value = cod;
     document.formulario.submit();
  }
  
  function ver_programa(cod) {
     document.formulario.verprgid.value = cod;
     document.formulario.submit();
  }  
  
    function excluir_programa(cod,descricao)
   {
   	if( window.confirm( "Confirma a exclusão do Programa "+descricao+" ?" ))
    {
   	   	  document.formulario.prog_n.value='exclui';
   	      document.formulario.prgid.value=cod;
   	      document.formulario.submit();
    }
   }  
  
  function popfun(prgid)
  {
	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/propostafusao_programa&acao=A&prgid='+prgid;
		window.open(e, "Associação_de_Responsáveis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=500,height=450");
  }
  function geraclone()
  {
	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/geraclone&acao=P';
		window.open(e, "GerarClone","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=500,height=250");
  }  
  function incluir_unid(cod)
  {

	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/ppafasequalitativa/acao/inclui_unid&acao=A&prgid='+cod;
		window.open(e, "Associação_de_Responsáveis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=500,height=450");
  }

  function tipoacao(cod)
  {
	document.formulario.submit();
  }
  function tipoorcamento(cod)
  {
	document.formulario.submit();
  }  
 
    
   function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
      
</script>

