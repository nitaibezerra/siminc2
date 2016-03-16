<?
/*
Sistema Simec
Setor responsavel: SPO-MEC
Desenvolvedor: Equipe Consultores Simec
Analista: Gilberto Arruda Cerqueira Xavier
Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Fabrício Mendonça(fabriciomendonca@gmail.com)
Modificacoes: Thiago Mata thiago_mata@yahoo.com.br 9 de novembro de 2006
Módulo:plantrabpje_apresentacao.inc
Finalidade: este arquivo monta o corpo do plano gerencial
*/

  function meunivel($id,$meunivel)
  {
  	global $db;

  	$sql = "select ptoid_pai,ptoid from monitora.planotrabalho where pjeid=".$_SESSION['pjeid']." and ptoid=$id";
  	$cod = $db->pegalinha($sql);
	  	if ($cod['ptoid_pai'])
  		{
  			// entao tenho pai 
  			$meunivel++;
  			return meunivel($cod['ptoid_pai'],$meunivel);
	  	}
  		else 
  		{
  			return $meunivel;
  		}
  }
  
/*
  $sql = "select ptoid from monitora.planotrabalho where ptoid_pai is null and pjeid=".$_SESSION['pjeid'];
  $regs = $db->carregar($sql);
  foreach ($regs as $reg)
  {
  	$sql = "update monitora.planotrabalho set ptonivel = 1 where pjeid=".$_SESSION['pjeid']." and ptoid = ".$reg['ptoid'];  
  	$db->executar($sql);
  	$sql = "select ptoid from monitora.planotrabalho where ptoid_pai = ".$reg['ptoid']." and pjeid=".$_SESSION['pjeid'];
    $reg2s = $db->carregar($sql);
      if (is_array($reg2s))
      {  
      	foreach ($reg2s as $reg2)
  		{
  	     	$sql = "update monitora.planotrabalho set ptonivel = 2 where pjeid=".$_SESSION['pjeid']." and ptoid = ".$reg2['ptoid'];
        	$db->executar($sql);
        	$sql = "select ptoid from monitora.planotrabalho where ptoid_pai = ".$reg2['ptoid']." and pjeid=".$_SESSION['pjeid'];
    		$reg3s = $db->carregar($sql);
      		if (is_array($reg3s))
     		 {  
		      	foreach ($reg3s as $reg3)
  				{
  	     			$sql = "update monitora.planotrabalho set ptonivel = 3 where pjeid=".$_SESSION['pjeid']." and ptoid = ".$reg3['ptoid'];
        			$db->executar($sql);
        			$sql = "select ptoid from monitora.planotrabalho where ptoid_pai = ".$reg3['ptoid']." and pjeid=".$_SESSION['pjeid'];
    				$reg4s = $db->carregar($sql);
      				if (is_array($reg4s))
     				 {  
		      			foreach ($reg4s as $reg4)
  						{
  	     					$sql = "update monitora.planotrabalho set ptonivel = 4 where pjeid=".$_SESSION['pjeid']." and ptoid = ".$reg3['ptoid'];
        					$db->executar($sql);
  						}
     		 		}
  				}
     		 }
  		}
      }
    
  }
 */
 
  
?>

<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/remedial.js"></script>
<script language="JavaScript" src="../includes/superTitle.js"></script>
<script language="JavaScript" src="plantrabpje_funcoes_js.js"></script>
<script language="JavaScript" src="plantrabpje_funcoes_js2.js"></script>
<script language="JavaScript">
	var limiteProjeto = 0<?=$limiteProjeto?>;
	function rep_img(){}
	
</script>
<script language="JavaScript" src="../includes/dateFunctions.js"></script>
<script language="JavaScript">
	var globalDataFimValida = '';
	function atualizaDataPelaDuracao( objInputDataInicio, objInputDataTermino, objInputIntervalo, strQuemMudara ){
		try{
			var intDuracao	= 0 + forceInt( objInputIntervalo.value );
			
			objDataInicio	= strDateToObjDate( objInputDataInicio.value , 'd/m/Y' , '/' );
			objDataTermino	= strDateToObjDate( objInputDataTermino.value , 'd/m/Y' , '/' );
			switch( strQuemMudara )
			{
				case 'objDataInicio':
				{
					// falta a funcao tira dias
					return;
					break;
				}
				case 'objDataTermino':
				{
					objDataTermino	= moreManyDays( objDataInicio , intDuracao );
					objInputDataTermino.value = objDateToStrDate( objDataTermino, 'd/m/Y' , '/' );
					break;
				}
				case 'objIntervalo':
				{
					intNovaDuracao = Math.ceil(( objDataTermino - objDataInicio )/( 60*60*24*1000 ));
					objInputIntervalo.value = intNovaDuracao;
					break;
				}
				default:
				{
					// ninguem será mudado //
					return;
				}
			}
			if( intNovaDuracao < 0 )
			{
				alert( 'Data Término não pode ser Anterior que Data Início.');
				
				objInputDataTermino.value = globalDataFimValida;
				objInputDataTermino.onchange();
			}
			else
			{	
				globalDataFimValida = objInputDataTermino.value;
			}
		}catch( e )
		{
		}
	}
	
</script>
<?
//Verifica se houve erro na alteração da data
if( $erroData )
{
	printf( "<script>alert( '%s' );</script>", $erroData[ 'mensagem' ] );
}
if( $strErro )
{
	printf( "<script>alert( '%s' );</script>", $strErro );
}
$statusprojeto= projetoaberto();
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,'');
if($_REQUEST['act2']=='alterar') {
	$sql= "select * from monitora.planotrabalho where ptoid=".$_REQUEST['ptoid'];
	$saida = $db->recuperar($sql,$res);
	if(is_array($saida)){
		foreach($saida as $k=>$v) ${$k}=$v;
		$_REQUEST['ptotipo']=$ptotipo;
	}
 } 
 else if ($_REQUEST['act2']=='planilha')
 {
 	 $pjeid = $_SESSION['pjeid'];

/*$sql= "select case when p.ptotipo='M' then 'chr(50)' end as teste, p.ptoordem as Ordem,case when p.ptotipo='P' then 'E' else 'M' end as Tipo, p.ptodsc as Titulo,p.ptodescricao as Descricao,to_char(p.ptodata_ini,'dd/mm/YYYY') as Inicio,to_char(p.ptodata_fim,'dd/mm/YYYY') as Termino  from monitora.planotrabalho p where pjeid=$pjeid and p.ptostatus='A' order by p.ptoordem";
*/
$sql="select p.ptoordem as ID, 
p.ptodsc as Titulo,p.ptodescricao as Descricao,(p.ptodata_fim - ptodata_ini) 
as duracao,to_char(p.ptodata_ini,'dd/mm/YYYY') as Inicio,to_char(p.ptodata_fim,'dd/mm/YYYY') as Termino, 
ptoid_antecessor as predecessor,ptonivel as Nivel ,ptodescricao as anotacoes  
from monitora.planotrabalho p where pjeid=".$_SESSION['pjeid']." and p.ptostatus='A' order by p.ptoordem";

 header('Content-type: application/xls');
 header('Content-Disposition: attachment; filename="planilha_simec.xls"');


	//header( 'Content-type: application/xls' );
	//header( 'Content-Disposition: attachment; filename="planilha_simec_projeto.xls"' );
	$db->sql_to_excel( $sql, 'planogerencial', '','' );
	exit();
}
 
 else {
	$ptocod = $_REQUEST['ptocod'];
	$ptodsc =$_REQUEST['ptodsc'];
	$unmcod = $_REQUEST['unmcod'];
	$ptoprevistoexercicio = $_REQUEST['ptoprevistoexercicio'];
	$ptosnsoma = $_REQUEST['ptosnsoma'];
	$ptosnpercent = $_REQUEST['ptosnpercent'];
	//$ptoanofim = $_REQUEST['ptoanofim'];
	$ptoid_antecessor = $_REQUEST[ 'ptoid_antecessor' ];
	$ptodata_ini = $_REQUEST[ 'ptodata_ini' ] ? date( "Y-m-d", gera_timestamp( $_REQUEST[ 'ptodata_ini' ] ) ) : '' ;
	$ptodata_fim = $_REQUEST[ 'ptodata_fim' ] ? date( "Y-m-d", gera_timestamp( $_REQUEST[ 'ptodata_fim' ] ) ) : '' ;
	

}

if ($_REQUEST['ptoid_pai'] or $ptoid_pai or $_REQUEST['ptopai'] )
{
	if ($_REQUEST['ptoid_pai'])  $ptoid_pai= $_REQUEST['ptoid_pai'];
	if ($_REQUEST['ptopai'])  $ptoid_pai= $_REQUEST['ptopai'];
		
	$sql="select ptosndatafechada as paifechado, to_char(ptodata_ini,'dd/mm/YYYY') as pjedataini,to_char(ptodata_fim,'dd/mm/YYYY') as pjedatafim from monitora.planotrabalho where ptoid=$ptoid_pai";
	$datas=$db->pegalinha($sql);
	$pjeinimt=$datas['pjedataini'];
	$pjefimmt=$datas['pjedatafim'];
	$paifechado=$datas['paifechado'];

}

$titulo_modulo='Estruturar o Plano de Trabalho do Projeto Especial <img src="../imagens/grupo.gif" title="Ver equipe do projeto" onclick="mostra_equipe()">  ';

monta_titulo($titulo_modulo,'');
//ordena_tarefas();
//novo_ajusta_ordem();
?>
<div align="center">
<center>

<?
// verifica se á coordenador de ação
$autoriza = false;
$coordpje=false;
$digit=false;

if ($db->testa_responsavel_projespec($_SESSION['pjeid'])) $coordpje = true;
// verifica se é digitador

if ($db->testa_digitador($_SESSION['pjeid'],'E')) $digit = true;
// verific se á super-usuário
if ($db->testa_superuser())   {
	$coordpje = true;
	$_SESSION[ 'coordpje' ] = true;
	$digit = true;
}

// verifica se o exercício está aberto para estruturar o simec
$sql= "select prsano from monitora.programacaoexercicio where prsano='".$_SESSION['exercicio']."' and prsstatus='A'";

$registro=$db->recuperar($sql);
if (! $coordpje and ! $digit and ! $visivel) {
    ?>
       <script>
       alert ('Vocá não tem acesso para monitorar o Plano Gerencial neste Projeto Especial!');
       //history.back();
       </script>
    <?
    exit();
}
?>
<?
	/**
	 * Seleciona o somatório das previsões de desembolso do projeto especial em questão
	 */
	$sqlLimite = 
		" select " .
	    " sum( coalesce( dpe.dpevalor, 0 ) ) as DESPESA " .
	    " from monitora.planotrabalho p " .
	  //  " inner join monitora.plantrabpje pa on pa.ptoid=p.ptoid and pa.pjeid = " . $_SESSION['pjeid'] .
	    " left join monitora.desembolso_projeto dpe on dpe.ptoid = p.ptoid " .
	    " where p.ptostatus='A' and p.ptoid_pai is null";
	
	if( $ptoid )
	{
		$sqlLimite .= " and p.ptoid <> ".$ptoid;
	}
	$totalGasto = $db->pegaUm( $sqlLimite );
	//calcula o valor que ainda pode ser utilizado nas atividades do projeto especial
	$limiteProjeto = $pjevlrano - $totalGasto;
	$usucpffiltro = $_REQUEST['usucpffiltro'];
	$metapafiltro = $_REQUEST['metapafiltro'];	
?>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css">
<form method="POST"  name="formulario" id="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="ptoid" value=<?=$ptoid?>>
<input type='hidden' name='exclui' value=0>
<input type='hidden' name='act'>
<input type='hidden' name='act2'>
<input type='hidden' name='nivel' value='<?=$_REQUEST['nivel']?>'>
<input type='hidden' name='ptopai'>
<input type='hidden' name='abrirarvore' value="<?=$_REQUEST['abrirarvore']?>">
<input type='hidden' name='usucpffiltro' value="<?=$_REQUEST['usucpffiltro']?>">
<input type='hidden' name='metapafiltro' value="<?=$_REQUEST['metapafiltro']?>">

<div id="camposDinamicos" style="visibility:hidden;margin:0; padding:0;"></div>


<center>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
  <tr>
    <td align='right' class="SubTituloDireita">Projeto :</td>
    <td><b><?=$pjecod?>&nbsp;-&nbsp;<?=$pjedsc?></b></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Período do Projeto:</td>
    <td><b><?=$pjedataini?>&nbsp; - &nbsp;<?=$pjedatafim?></b></td>
  </tr>  
  <tr>
     <td align="right">GRÁFICO DE GANTT
     </td>
     <td>
     <input type="button" class='botao' value='Ver Gráfico' title='Ver Gráfico.' onclick="exibe_grafico('<?=$ptoid?>')">     <input type="button" class='botao' value='Exporta XLS' title='Exporta o Plano Gerencial para o formato XLS.' onclick="mostra_xls('<?=$ptoid?>')">
     <br> <input type="radio" name="rbnivel" value="1">Nível 1<input type="radio" name="rbnivel" value="2">Nível 2 <input type="radio" name="rbnivel" value="3">Nível 3<input type="radio" name="rbnivel" value="1000">Nível 4 ou maior
     </td>
     </tr>
<?
if ($coordpje or $digit )
{?>
    <tr><td align='right' class="SubTituloDireita">Incluir novo item:</td><td>
    <?
   if (projetoaberto()) {?>
     <input type="button" class='botao' value='Inserir Tarefa' title='Inclusão de uma tarefa no projeto.' onclick="incluirtarefa('0')">
     <? } else {?>
     <font color="red">O Projeto está Concluído ou Cancelado</font>
     <?}?>
     
     </td></tr>

<? } 
	?>
   
<tr >
   <td align='right' class="SubTituloDireita">Filtros</td>
   <td><? $sql = "select distinct u.usucpf as CODIGO,u.usucpf ||'-'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum as DESCRICAO, u.usunome from seguranca.usuario u inner join monitora.usuarioresponsabilidade ur on ur.usucpf=u.usucpf and ur.pjeid=$pjeid where u.usucpf in (select pu.usucpf from seguranca.perfilusuario pu where pu.pflcod=51) or u.usucpf in (select us.usucpf from seguranca.usuario_sistema us where us.pflcod=51) and u.usustatus='A' order by usunome ";
	$db->monta_combo("usucpffiltro",$sql,'S',"Selecione o Responsável",'','','',300);
   print "&nbsp;&nbsp; Tarefa agregadora de nível 1 (Digite a ordem) ";
   print campo_texto('metapafiltro','N','S','',16,14,'######','','left','Digite o número de ordem da tarefa agregadora de nível 1!');
	    
	    print '<br>';
	    
	    ?>
	    <input type="button" name="btfiltro" value="Filtrar" onclick="filtra()">
	    </td>

 </tr>  
  <tr ><td align="right">Legenda:</td>
  <td><b><img border='0' width=8 heigth=11 src='../imagens/ppa.gif' title='Subação proveniente do PPA'> - Proveniente do PPA</b>
  <?
if (! $_REQUEST['abrirarvore']) { ?>
  <input type="button" name="btabrirarvore" value="Abrir árvore de tarefas " onclick="abrir_arvore('1')" class="botao">
  <?} else {
  	?>
  	  <input type="button" name="btabrirarvore" value="Fechar árvore de tarefas " onclick="abrir_arvore('0')" class="botao">
  	<?}?>
  
  </td>
         </tr>
       
    </table>
<?
$sql = "select ptonivel from monitora.planotrabalho where pjeid=$pjeid and ptostatus='A' order by ptonivel desc limit 1";
$maiornivel=$db->pegaum($sql);

$sql = "select p.ptoid,ptoid_pai, p.ptoorigemppa, p.ptotipo,p.ptocod,p.ptoordem,p.ptoid_antecessor,p.ptodata_ini, p.ptodata_fim,p.usucpf,p.ptonivel,p.ptoprevistoexercicio as previsto, case when p.ptotipo='M' then 'M' when p.ptotipo='P' then 'E' end as tipo, ptodsc,to_char(ptodata_ini,'dd/mm/yyyy') as inicio, to_char(ptodata_fim,'dd/mm/yyyy') as termino from monitora.planotrabalho p where p.ptostatus='A' ";
//and p.ptoid in 
//(select ptoid from monitora.plantrabpje where pjeid=".$_SESSION['pjeid'].") ";
		
if ($_REQUEST['metapafiltro'] or $_REQUEST['usucpffiltro'])
{
	if ($_REQUEST['metapafiltro'])
	{
		$sql2 = "select ptoid from monitora.planotrabalho where ptoordem=".$_REQUEST['metapafiltro']." and pjeid=".$_SESSION['pjeid']. " and ptostatus='A'";
		$filtroid=$db->pegaum($sql2);
        $sql .= " and p.ptoid in (select ptoid from  SELECT_ALL_CHILDS_FROM_PTO( $filtroid )) or p.ptoid=$filtroid  ";
	}	
	if ($_REQUEST['usucpffiltro'])
	{
		$sql .= " and p.usucpf = '".$_REQUEST['usucpffiltro']."'";
	}		
}
$_SESSION['sql_gantt']=$sql;
$_SESSION['sql_gantt2']="  order by ptocod::integer * 10 ^($maiornivel-ptonivel+1)";

$sql = $sql ." order by ptocod::integer * 10 ^($maiornivel-ptonivel+1) ";


$rs = @$db->carregar( $sql );

	?>

<table style="width:770px;" align='center' border="0" cellspacing="0" cellpadding="0" class="listagem">
<thead>
    <tr>
      <td align="right" class="title" colspan="2" style="width:100px; padding:3px;"><strong></strong></td>      
      <td class="title" style="width:500px;padding:3px;" ><strong>Título </strong></td>
    </tr>
</thead>
<tbody>
<?
if (  $rs && count($rs) > 0 )
{
	$i=0;
	foreach ( $rs as $linha )
	{
		foreach($linha as $k=>$v) ${$k}=$v;
		if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
		$nivel = $db->busca_pai($ptoid,0);
		//dbg($nivel);
		// exibe status
		$sqlStatus = "select t.tpsdsc as status, t.tpscor as cor from public.tiposituacao t inner join monitora.execucaopto e on e.tpscod = t.tpscod where e.ptoid=".$ptoid." order by e.expdata desc limit 1";
		$rsStatus = @$db->recuperar( $sqlStatus );
		$status = $rsStatus[ "status" ] ? $rsStatus[ "status" ] : "S/ avaliação";
		$cor = $rsStatus[ "cor" ] ? $rsStatus[ "cor" ] : "black";
		$sqlAlt =
		" select " .
		" pt.ptodsc, " .
		" pt.ptoid, pt.ptosnaprovado," .
		" epobs.observacao, " .
		" pt.ptoprevistoexercicio as previsto, " .
		" sum( ep.exprealizado ) as realizado, " .
		" sum( ep.expfinanceiro ) as gasto, " .
		" ( ( sum( ep.exprealizado ) / pt.ptoprevistoexercicio ) * 100 ) as porcentagem " .
		" from monitora.planotrabalho pt " .
		" inner join monitora.execucaopto ep using ( ptoid ) " .
		" left join ( " .
		" select expobs as observacao, ptoid from monitora.execucaopto where ptoid = " . $ptoid . " order by expdata desc limit 1 " .
		" ) epobs using ( ptoid ) " .
		" where ptoid = " . $ptoid .
		" group by pt.ptodsc, pt.ptoid, pt.ptosnaprovado,epobs.observacao, pt.ptoprevistoexercicio";
		
		$dadosAlt = $db->recuperar( $sqlAlt );
		$txtAlt =
		"Previsto: " . formata_valor( $dadosAlt['previsto'], 0 ) . "<br/>" .
		"Executado: " . formata_valor( $dadosAlt['realizado'], 0 ) . "<br/>" .
		"Gasto: R$ " . formata_valor( $dadosAlt['gasto'], 0 ) . "<br/>" .
		"Percentual: " . formata_valor( $dadosAlt['porcentagem'], 2 ) . "%";
		if ( $dadosAlt['observacao'] )
		{
			$txtAlt .= "<br/><br/>" . $dadosAlt['observacao'];
		}
		$status = '<font color="'. $cor . '">' . $status . '</font>';
//		$status = '<span onmouseover="return escape(\'' . $txtAlt .'\')">' . $status . '</span>';
		$status = '<span onmouseover="SuperTitleOn( this , \'' .  htmlentities( $txtAlt ) . '\')" onmouseout="SuperTitleOff( this )" >' . $status . '</span>';
		// FIM exibe status

		// para cada registro devo verificar se ele é uma etapa ou macro -etapa
		// se for uma macro etapa, coloco e
		if ($ptoorigemppa=='t')
		{
			$ppa="<img border='0' width=8 heigth=11 src='../imagens/ppa.gif' title='Subação proveniente do PPA'>&nbsp;&nbsp;";
		} else
		$ppa='';

		if ($nivel ==0 )
		{
    	?>
       <tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
          <td style="width:60px; text-align:left; padding:3px;">
          <? if ($statusprojeto) {?>
            <img border="0" src="../imagens/gif_inclui.gif" title="Incluir Tarefa dentro desta Tarefa." onclick="incluirtarefa('<?=$ptoid?>')">&nbsp;&nbsp;
            <?}?>
            <img border="0" src="../imagens/alterar.gif" title="Editar a Tarefa." onclick="alterartarefa('<?=$ptoid?>')">
            <? if (! $ppa and $statusprojeto) { ?>
            &nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir a Tarefa." onclick="excluir_pto('<?=$ptoid?>','<?=$ptocod?>',1)">
            <?}?>
            
          </td>
          <td style="width:40px; padding:3px;text-align:right;"><?=$ppa.$ptoordem?></td>
		  <td  style="width:500px;text-align:left;padding:3px;" onclick="abreconteudo('geral/listamacroetapa.php?ptoid=<?=$ptoid?>','<?=$ptoid?>')">
		  
		  <? if (busca_filho($ptoid)) { ?>
     		  <img src="../imagens/mais.gif" name="+" border="0" id="img<?=$ptoid?>">
		  <?}?>
		  
		  &nbsp;&nbsp;<b><?=mostracod($ptoid).'-'.$ptodsc?></b>		  </td>
	</tr>
	<tr bgcolor="<?=$marcado?>"> 
		<td style="width:770px; padding:3px;" colspan="3" id="td<?=$ptoid?>"></td>
		<? //verificação se a atividade é pai da atividade com erro de alteração de data
		if( $erroData[ "ptoid_pai" ] == $ptoid or $_REQUEST['abrirarvore']==1)
		{
		?>
			<script type="text/javascript">
				abreconteudo('geral/listamacroetapa.php?ptoid=<?=$ptoid?>','<?=$ptoid?>');
			</script>
		<?
		}//Fim verificação se a atividade é pai da atividade com erro de alteração de data
		?>
	</tr>
	<?} 
	else if ($nivel==1)
	{
        	?>
     <tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
        <td style="width:60px; text-align:left; padding:3px;">
                  <? if ($statusprojeto) {?>
            <img border="0" src="../imagens/gif_inclui.gif" title="Incluir Tarefa dentro desta Tarefa." onclick="incluirtarefa('<?=$ptoid?>')">&nbsp;&nbsp;
            <?}?>
			<img border="0" src="../imagens/alterar.gif" title="Editar a Tarefa." onclick="alterartarefa('<?=$ptoid?>')">
            
            <? if (! $ppa and $statusprojeto) { ?>
            &nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir a Tarefa." onclick="excluir_pto('<?=$ptoid?>','<?=$ptocod?>',0)">
            <?}?>  
            
          </td>
          <td style="width:40px; padding:3px;text-align:right;"><?=$ppa.$ptoordem?></td>
		  <td  style="width:500px;text-align:left;padding:3px;" onclick="abreconteudo('geral/listamacroetapa.php?ptoid=<?=$ptoid?>','<?=$ptoid?>')">		  
		  
		  <? if (busca_filho($ptoid)) { ?>
     		  <img src="../imagens/mais.gif" name="+" border="0" id="img<?=$ptoid?>">
		  <?}?>
		  
		  &nbsp;&nbsp;<b><?=mostracod($ptoid).'-'.$ptodsc?></b>		  </td>
	 </tr>
	 <tr bgcolor="<?=$marcado?>">
		 
		 <td style="width:770px; padding:3px;" colspan="8" id="td<?=$ptoid?>"></td>
	 </tr>
	        <?}

	        $i++;}
}
		?>
	  
</tbody>
</table>
    </center>
  </div>
</form>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
<script>
  function mostra_equipe()
  {
  	 e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/verequipe&acao=A";
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=800,height=300'");
  }
  
  function mostra_xls(cod)
  {
  	rbn = 1000;
  	if (formulario.rbnivel[0].checked) rbn=1
  	else if (formulario.rbnivel[1].checked) rbn=2
  	else if (formulario.rbnivel[2].checked) rbn=3  
  	else if (formulario.rbnivel[3].checked) rbn=1000;

  	if (! cod) cod=0;
  	e="monitora.php?modulo=sistema/geral/mostraxls&acao=A&cod="+cod+"&nivel="+rbn;
    window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=10,height=10'");
     
     
  }
  
  function incluirtarefa(cod) {
	 e = "monitora.php?modulo=principal/projespec/plantrabpje_insercao&acao=I&id="+cod;
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=300'");
}
  
function alterartarefa(cod) {
	 e = "monitora.php?modulo=principal/projespec/plantrabpje_edicao&acao=A&id="+cod;
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=600'");
}

function filtra()
{
	document.formulario.submit();
}
</script>
<script language="JavaScript">
atualizaDataPelaDuracao( document.formulario.ptodata_ini , document.formulario.ptodata_fim, document.formulario.duracao , 'objIntervalo' );
</script>