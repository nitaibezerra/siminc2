<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:monitoraacao.inc
   finalidade: permitir o monitoramento de Ação
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['prgid'])
	$_SESSION['prgid'] = $_REQUEST['prgid'];
else
	$_REQUEST['prgid'] = $_SESSION['prgid'];

if ($_REQUEST['acaid'])
	$_SESSION['acaid'] = $_REQUEST['acaid'];
else
	$_REQUEST['acaid'] = $_SESSION['acaid'];

/*
 * Correção Alexandre Dourado 16/11/2009
 *  - Validando se existe alguma ação(acaid) selecionada 
 */
if(!$_SESSION['acaid']) {
	echo "<script>
			alert('Ação não selecionada corretamente');
			window.location='monitora.php?modulo=inicio&acao=C';
		  </script>";	
	exit;
}


if ($_REQUEST['tipograva']== 'A' and $_REQUEST['avptexto']=='100000')
{
  $erro_total=0;
  foreach($_POST as $k=>$v)
  {
    if (substr($k,0,10)=='avaliacao_')
     {
        $erro = 0;
        ${$k}=$v;
        $parte = substr($k,10,strlen($k)-10);
        $refer = substr($parte,0,strpos($parte,'_'));
        $parte2 = substr($parte,strpos($parte,'_')+1,100);
        $codigo = substr($parte2,0,strpos($parte2,'_'));
        $ordem = substr($parte2,strpos($parte2,'_')+1,100);

        $texto = str_replace("'","''",$texto);
        
        $cor =  $_REQUEST['corcodav'.$ordem];
	    $sit =  $_REQUEST['tpscodav'.$ordem];
	    $exp =  $_REQUEST['exprealizado'.$ordem];
	    $texto = stripslashes($_REQUEST['avaliacao_'.$refer.'_'.$codigo.'_'.$ordem]);
    	$texto = str_replace("'","''",$texto);
        //$texto = strip_tags($texto,'<p>,<b>,<i');
        $texto = stripslashes($texto);
	    if (ereg_replace("<[^>]*>","",$texto) == '') {$erro = 1;$erro_total=1;};
        if ($exp == '') {$erro = 1;$erro_total=1;};
        if ($sit == '') {$erro = 1;$erro_total=1;};
        if ($cor == '') {$erro = 1;$erro_total=1;};

        if (! $erro)
        {
           if ($codigo == 0)
           {
           	   // aqui deve ser testado se já existe um registro com os mesmos dados. Este erro está ocorrendo em algumas regiões do Brasil
           	   $sql = "select avpid from monitora.avaliacaoparecer where acaid=".$_SESSION['acaid']." and refcod=$refer";
           	   $duplicidade=0;
           	   $duplicidade=$db->pegaUm($sql);
           	   if (! $duplicidade)
               $sql= "insert into avaliacaoparecer (tpaid,avporigem,refcod,usucpf,corcod,tpscod,acaid,avptexto,avpstatus,avpdata) values (1,1,".$refer.",'".$_SESSION['usucpf']."',".$_POST['corcodav'.$ordem].",".$_POST['tpscodav'.$ordem].",".$_SESSION['acaid'].",'".$texto."',"."'A','".date('Y/m/d')."')";
               else 
               $sql='';
           }
           else
               $sql= "update avaliacaoparecer set refcod=".$refer.",usucpf='".$_SESSION['usucpf']."',corcod=".$_POST['corcodav'.$ordem].",tpscod=".$_POST['tpscodav'.$ordem].",acaid=".$_SESSION['acaid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$codigo;
           if ($sql) 
           {
                $db->executar($sql);
           		if ($_POST['exprealizado'.$ordem] <>'-')
           		{
			  		$sql = "select * from execucaopto where refcod=".$refer." and acaid=".$_SESSION['acaid'];
		     		if($db->eof($sql))
		     		{
			    		$sql = "insert into execucaopto (exprealizado, usucpf, acaid, refcod) values (".$_POST['exprealizado'.$ordem].",'".$_SESSION['usucpf']."',".$_SESSION['acaid'].",".$refer.")";
		     		}
             		else
		     		{
       		     		$sql = "update execucaopto set exprealizado=".$_POST['exprealizado'.$ordem].", usucpf='".$_SESSION['usucpf']."', expdata='".date('Y/m/d')."' where acaid=".$_SESSION['acaid']." and refcod=".$refer;
	         		}
		    		 $db->executar($sql);
	       		}
        	}
         }
     }
  }

  if ($erro_total && !$_REQUEST['boComboReferencia'])
  {
	   ?>
	      <script>
	         alert ('Há campos vazios que não foram gravados.\nA avaliação, a situação e a cor não podem ficar em branco.\nVerifique os dados.\nSomente registros completos são gravados corretamente.!');
	      </script>
	   <?
  }
  if (! $erro or $_REQUEST['refcod']=='x')
	{
       $db->commit();
       $db->sucesso($modulo, '&refcod='.$_REQUEST['refcod']);
	   exit();
	}
}

if ($_REQUEST['refcod']=='x') {
	echo "<script>alert('Clique no botão \"Gravar todas as avaliações\" no rodapé da página para salvar todos os períodos.');</script>";
}

if ($_REQUEST['tipograva']== 'LA' or ($_REQUEST['tipograva']== 'A' and $_REQUEST['avptexto']<>'100000') && !$_REQUEST['boComboReferencia'])
{
	$cor =  $_REQUEST['corcodav'.$_REQUEST['avptexto']];
	$sit =  $_REQUEST['tpscodav'.$_REQUEST['avptexto']];
	$exp =  $_REQUEST['exprealizado'.$_REQUEST['avptexto']];
	$texto = stripslashes($_REQUEST['avaliacao_'.$_REQUEST['refren'].'_'.$_REQUEST['doccod'].'_'.$_REQUEST['avptexto']]);
	$texto = str_replace("'","''",$texto);
//	$texto = strip_tags($texto);

    if (ereg_replace("<[^>]*>","",$texto) == '') {
	   ?>
	      <script>
	         alert ('A Avaliação não pode estar em branco.');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
	   if ($exp == '') {
	   ?>
	      <script>
	         alert ('Informe o Executado no Período. Campo Obrigatório');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
       if ($sit == '') {
	   ?>
	      <script>
	         alert ('Escolha uma situação. Campo obrigatório');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
       if ($cor == '') {
	   ?>
	      <script>
	         alert ('Escolha uma das cores. Campo Obrigatório');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
		if ($_REQUEST['tipograva']== 'A')
		{
		     // é uma avaliação
		     // verifico se o cod é zero, se for, será um insert, senão será update
		    if ($_REQUEST['doccod']== '0')
		     {
		     	// aqui deve ser testado se já existe um registro com os mesmos dados. Este erro está ocorrendo em algumas regiões do Brasil
           	   $sql = "select avpid from monitora.avaliacaoparecer where acaid=".$_SESSION['acaid']." and refcod=".$_REQUEST['refren'];
           	   $duplicidade=0;
           	   $duplicidade=$db->pegaUm($sql);
           	   if (! $duplicidade)
		       $sql= "insert into avaliacaoparecer (tpaid,avporigem,refcod,usucpf,corcod,tpscod,acaid,avptexto,avpstatus,avpdata) values (1,1,".$_REQUEST['refren'].",'".$_SESSION['usucpf']."',".$cor.",".$sit.",".$_SESSION['acaid'].",'".$texto."',"."'A','".date('Y/m/d')."')";
               else 
               $sql=0;
               if ($sql) $db->executar($sql);
		     }
		     else
		     {
		       $sql= "update avaliacaoparecer set refcod=".$_REQUEST['refren'].",usucpf='".$_SESSION['usucpf']."',corcod=".$cor.",tpscod=".$sit.",acaid=".$_SESSION['acaid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$_REQUEST['doccod'];
			   //print $sql.'<br>';
			   $db->executar($sql);

		     }
		}
		if ($_REQUEST['tipograva']== 'LA')
			{
      $sql= "update avaliacaoparecer set avpliberada='t',refcod=".$_REQUEST['refren'].",usucpf='".$_SESSION['usucpf']."',corcod=".$cor.",tpscod=".$sit.",acaid=".$_SESSION['acaid'].",avptexto='".$texto."',avpdata='".date('Y/m/d h:i:s')."' where avpid=".$_REQUEST['doccod'];
	 $db->executar($sql);
	 }
		//Grava o executado
		if ($exp<>'-' and $sql)
		{
			 $sql = "select * from execucaopto where refcod=".$_REQUEST['refren']." and acaid=".$_SESSION['acaid'];
			 if($db->eof($sql))
			{
				$sql = "insert into execucaopto (exprealizado, usucpf, acaid, refcod) values (".$exp.",'".$_SESSION['usucpf']."',".$_SESSION['acaid'].",".$_REQUEST['refren'].")";
				$db->executar($sql);
			}else
			{
				$sql = "update execucaopto set exprealizado=".$exp.", usucpf='".$_SESSION['usucpf']."', expdata='".date('Y/m/d')."' where acaid=".$_SESSION['acaid']." and refcod=".$_REQUEST['refren'];
				$db->executar($sql);
			}
		}
	 $db->commit();
	 $db->sucesso($modulo, '&refcod='.$_REQUEST['refcod']);
	 exit();
}
if ($_REQUEST['tipograva']== 'BA')
{
     $sql= "update avaliacaoparecer set avpliberada='f' where avpid=".$_REQUEST['doccod'];
	 $db->executar($sql);
	 $db->commit();
	 $db->sucesso($modulo, '&refcod='.$_REQUEST['refcod']);
	 exit();
}

include  APPRAIZ."includes/cabecalho.inc";

?>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script type="text/javascript">
	tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		editor_selector : "txareanormal",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",

		// Theme options
		theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,forecolor,backcolor,fontsizeselect,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist, code",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_buttons4 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		theme_advanced_statusbar_location : "",
		theme_advanced_resizing : true,
		onchange_callback : "atualizaCamposTextArea",

		// Example content CSS (should be your site CSS)
		content_css : "css/content.css",

		// Drop lists for link/image/media/template dialogs
		template_external_list_url : "lists/template_list.js",
		external_link_list_url : "lists/link_list.js",
		external_image_list_url : "lists/image_list.js",
		media_external_list_url : "lists/media_list.js",

		// Replace values for the template plugin
		template_replace_values : {
			username : "Some User",
			staffid : "991234"
		}
	});
	
</script>
<script language="JavaScript">
var listaTextArea = new Array();

function atualizaCamposTextArea()
{
	var textarea_id = '';
	var impressao_id = '';
	var nome = '';
	var elemento = null;
	for ( var i = 0; i < listaTextArea.length; i++ )
	{
		nome = listaTextArea[i];
		textarea_id = nome;
		impressao_id = 'print_' + nome;
		elemento = document.getElementById( impressao_id );
		if ( elemento )
		{
			//elemento.innerHTML = tinyMCE.getInstanceById( textarea_id ).getHTML();
			elemento.innerHTML = tinyMCE.get(textarea_id).getContent()
		}
	}
}

</script>
<br>

<?
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Avaliação de Ação';
monta_titulo($titulo_modulo,'');
$sql= "select a.*, u.*, m.unmdsc, p.prodsc from acao a inner join unidade u on a.unicod=u.unicod left join public.unidademedida m on a.unmcod=m.unmcod left join produto p on a.procod=p.procod where a.acastatus='A' and a.acaid = ".$_REQUEST['acaid'];
if (! $_REQUEST['acaid'] or $_REQUEST['acaid']=='') {
	?>
	<script>alert('Ocorreu um problema ao abrir a página. Tente novamente!');
	history.back();</script>
	<?
	$db->close();
	exit();
}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

if ($nlinhas < 0 ){
	?>
	<script>alert('Ação Inexistente!');
	history.back();</script>
	<?
	$db->close();
	exit();}
else {
  $res =  @$db->carrega_registro($RS,0);
  
  // a alteração abaixo foi feita a pedido do Prof Leo e deve ser retirada logo para 2007
	$acasnbgu = $res['acasnbgu'];
}
// Transforma em variáveis todos os campos do array


if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

if (! $_REQUEST['refcod'])
{
  $sql = "select refcod from referencia where refsnmonitoramento='t' and refdata_inicio < now()::date and refano_ref='".$_SESSION['exercicio']."' order by refano_ref desc,refmes_ref desc";
 $RS = $db->record_set($sql);
$res =  @$db->carrega_registro($RS,0);
$_REQUEST['refcod']= $res[0];

}

?>
<form name="formulario" method="post">
<input type=hidden name="boComboReferencia" value="0">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="refren" value=0>
<input type=hidden name="doccod" >
<input type=hidden name="tipograva">
<input type=hidden name="procura">
<input type=hidden name="avptexto" value=0>

<center>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
 		<?
 		@$db -> cabecalho_acao($_SESSION['acaid']);
 		
			$sql = sprintf(
				"select max(rofdatainclusao) as dataatu 
				 from financeiro.execucao 
				 where prgcod = '%s' and acacod = '%s' and unicod = '%s' and loccod = '%s' and rofano = '%s'",
				$prgcod, $acacod, $unicod, $loccod, $_SESSION['exercicio']);
			$dataatualizacaobdexecucao = @$db->carregar( $sql );
			if($dataatualizacaobdexecucao) {
				$dataatualizacaobdexecucao = current($dataatualizacaobdexecucao);
				$dataatualizada = $dataatualizacaobdexecucao['dataatu'];
			}
			
			$sql = sprintf(
				"select rofmes, max(rofdatainclusao) - interval '1 day' as dataatu, sum(rofdot_ini) as rofdot_ini, sum(rofautorizado) as rofautorizado,  sum(rofempenhado) as empenhado,
				 sum(rofliquidado_favorecido) as rofliquidado_favorecido, sum(rofpago) as rofpago
				 from financeiro.execucao 
				 where prgcod = '%s' and acacod = '%s' and unicod = '%s' and loccod = '%s' and rofano = '%s'
				 group by rofmes, prgcod,acacod, unicod,loccod",
				$prgcod, $acacod, $unicod, $loccod, $_SESSION['exercicio']);

			$dadosfinanceiroscompletos = @$db->carregar( $sql );
			$dadofinanceiro = array();
			if($dadosfinanceiroscompletos) {
				foreach($dadosfinanceiroscompletos as $linhafin) {
					$dataatualizada = $linhafin['dataatu'];
					$dadofinanceiro['rofdot_ini'] += $linhafin['rofdot_ini'];
					$dadofinanceiro['rofautorizado'] += $linhafin['rofautorizado'];
					$dadofinanceiro['empenhado'] += $linhafin['empenhado'];
					$dadofinanceiro['rofliquidado_favorecido'] += $linhafin['rofliquidado_favorecido'];
					$dadofinanceiro['rofpago'] += $linhafin['rofpago'];
					$dadofinanceiropormes[$linhafin['rofmes']] = array("empenhado" => $linhafin['empenhado'],
																	   "liquidado" => $linhafin['rofliquidado_favorecido'],
																	   "pago" 	   => $linhafin['rofpago']);
				}
				$dadofinanceiropormes['totalautorizado'] = $dadofinanceiro['rofautorizado']; 	
			}
		?>
		<tr>
			<td align='right' class="subtitulodireita">Dados Financeiros:</td>
			<td>
				<table class="tabela">
					<tr>
						<td class="SubTituloDireita">Dotação Inicial</td>
						<td class="SubTituloDireita">Dotação ( Lei + Créditos )</td>
						<td class="SubTituloDireita">Empenhado</td>
						<td class="SubTituloDireita">Liquidado</td>
						<td class="SubTituloDireita">Pago</td>
					</tr>
					<tr>
						<?php foreach( $dadofinanceiro as $valor ): ?>
							<td align="right">R$ <?= number_format($valor, 2, ',', '.'); ?></td>
						<?php endforeach; ?>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td align='right' class="subtitulodireita">Dados Atualizados até:</td>
			<td>
			<?php
			echo formata_data( $dataatualizada );
			?>
			</td>
		</tr>

<tr><td align='right' class="SubTituloDireita">Período de Referência:</td>
<td>
<?

$refcod = $_REQUEST['refcod'];
$sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from referencia where refdata_limite_avaliacao_aca is not null and refsnmonitoramento='t'   and refano_ref='". ( $_SESSION['exercicio'] ) ."'   order by refano_ref,refmes_ref desc";
$db->monta_combo("refcod",$sql2,'S',"Escolha o período de referência",'Escolhe_referencia','Todos os Períodos de Referência');
?>
</td></tr>
  </table>
  <?
  /////////////////////Grafico evolucão produto
//verifica se pode ser acompanhado (Se tem produto pode!) nao pode o procod 0,3
if (!in($procod,array('0','3'))) {
	//Recupera os dados da ação
	  $sql = "SELECT aca.acasnmetanaocumulativa, aca.acaid, aca.acacod, aca.procod, pro.prodsc, 
dad.fisqtdeprevistoano as previsto, u.unmdsc,
case when aca.acasnmetanaocumulativa='f' then sum(exe.exprealizado) else max(exe.exprealizado) end as realizado
 FROM monitora.acao aca 
left join public.unidademedida  u on u.unmcod=aca.unmcod 
left join produto pro on aca.procod=pro.procod 
left  join monitora.dadofisico dad on aca.acaid=dad.acaid 
left join monitora.execucaopto exe on aca.acaid=exe.acaid
left  join monitora.referencia r on exe.refcod=r.refcod and r.refano_ref='".$_SESSION['exercicio']."' 
WHERE aca.acaid=".$_SESSION['acaid']." and aca.acasnrap='f' group by aca.acasnmetanaocumulativa, aca.acaid, aca.acacod, aca.procod, pro.prodsc, 
dad.fisqtdeprevistoano, u.unmdsc";

	  $RS = $db->record_set($sql);
	  $nlinhas = $db->conta_linhas($RS);
	  if ($nlinhas >= 0)
	  {
	  	for ($i=0; $i<=$nlinhas;$i++){
			$res = $db->carrega_registro($RS,$i);
		  	foreach($res as $k=>$v) ${$k}=$v;}
	  }
  
	  $porcentorealizado = 0;
	  $porcentoexecutado = 0;
	  $porcentoplanejado = 0;
	  $planejado = 0;
	  $fisqtdeprevistoano = $previsto;
	  
	  if ($acasnmetanaocumulativa=='t')
		  	{ // é não cumulativa    *  Private web browsing
               $tipometa = 'Não cumulativa';
	        } else
	        {  // é cumulativa
                $tipometa = 'Cumulativa';
	        }
	 
	  if ($previsto>0) {
	    $porcentorealizado = $realizado*100/$previsto;
	    if ($porcentorealizado > 100) $porcentorealizado2 = 100;
	    else $porcentorealizado2 = $porcentorealizado;
	    $porcentorealizado  = number_format($porcentorealizado, 2, '.', '');
	    }
	  $porcentoplanejado = 0;
	  $planejado = 0;
	  $porcentoexecutado = 100 - $porcentorealizado;
	  
	  
	   $cabecalhoproduto = "Produto: <strong>".$prodsc."</strong>&nbsp;|&nbsp;Unid. de Medida: <strong>".$unmdsc."</strong>&nbsp;|&nbsp;Meta: <strong>".$tipometa."</strong>";	  
	  

}
	   ?>
	   <TABLE border="0" cellspacing="0" cellpadding="2" class="listagem" align="center" width="95%">
	<THEAD>
		<tr>
		<TD colspan="16" align="center" style="color:#000000;">
		<? echo (($cabecalhoproduto)?$cabecalhoproduto:'&nbsp;'); ?>
		</TD>
		</tr>
	</THEAD>
	<TBODY>
			    <TR style="color:#808080;" bgcolor="#f6f6f6">
				<td>Realizado</td>
				<?php 
					if($_SESSION['exercicio'] > 2011){
						$tipo_ano = 2;
					}else{
						$tipo_ano = 12;	
					}
				?>
				<? for($i = 1; $i <= $tipo_ano; $i++) { ?>
	    			<TD align="right" width="55" style="font-size: 9px"><?=$i.'/'.$_SESSION['exercicio']?></TD>
				<? } ?>
				<td align="right" >Total</td>
				<td align="right" >Previsto</td>
				<td align="right" nowrap="nowrap">% Exec.</td>
			   </TR>
	
			   
			    <TR style="height:30px;" bgcolor="#ffffff">
				<TD align="right"><font color="#3366ff">Físico</font></TD>
				<?
				$meses = array();
				$sql = "select r.refcod, 
							   r.refmes_ref, 
							   e.exprealizado, 
							   e.expobs, 
							   e.tpscod, 
							   t.tpscod, 
							   case when t.tpsdsc is null then 'Não Informado' else t.tpsdsc end as tpsdsc, 
							   case when c.cordsc is null then 'branco' else trim(c.cordsc) end as cordsc 
							   from referencia r 
							   left join avaliacaoparecer a on r.refcod=a.refcod and a.acaid=".$_SESSION['acaid']." and a.avpliberada='t' 
							   left join execucaopto e on r.refcod=e.refcod and e.acaid=".$_SESSION['acaid']." 
							   left join tiposituacao t on a.tpscod=t.tpscod 
							   left join cor c on a.corcod=c.corcod 
							   where r.refdata_limite_avaliacao_aca is not null and 
							   		 r.refsnmonitoramento='t' and 
							   		 r.refano_ref='".$_SESSION['exercicio']."' 
							   order by refano_ref,refmes_ref";
				$RS2 = $db->record_set($sql);
				$nlinhas2 = $db->conta_linhas($RS2);
				$totalrealizado = 0;
				if ($nlinhas2 >= 0) {
					for ($j=0; $j<=$nlinhas2;$j++){
						array_push( $meses, (integer) $refmes_ref );
						$res2 = $db->carrega_registro($RS2,$j);
					  	foreach($res2 as $k=>$v) ${$k}=$v;
						if (!$exprealizado) $txtexprealizado = '-'; else {$txtexprealizado = $exprealizado; $totalrealizado += $txtexprealizado; }?>
				<td align="right" style="color:#3366ff; font-size: 9px" title="<?=$tpsdsc?>" onClick="location.href='monitora.php?modulo=principal/acao/monitoraacao&acao=A&refcod=<?=$refcod?>';" onMouseOver="this.bgColor='#ffffcc';" onMouseOut="this.bgColor='';">&nbsp;<?=formata_numero($txtexprealizado)?><br>
				<img border="0" width="25" height="10" src="../imagens/av_<?=strtolower($cordsc)?>.gif"></td>
				<?		}
					}
					for($j = $nlinhas2; $j <= 10; $j++) { ?>
						<td align="right" style="color:#3366ff; font-size: 9px">-</td> 					
				<?  } ?>
			    <TD align="right" style="color:#3366ff; font-size: 9px" ><strong><?=(($realizado)?formata_numero($realizado):'-');?></strong></TD>
				<TD align="right" style="color:#3366ff; font-size: 9px" ><font color="#006108"><strong><?=(($previsto)?formata_numero($previsto):'-')?></strong></font></TD>
				<td align="right" style="color:#000000; font-size: 9px" >
					<?= (($porcentorealizado)?number_format($porcentorealizado, 0, ',', '.').'%':'-'); ?>
				</td>
			   </TR>
		<tr style="height:30px" bgcolor="#f1f1f1">
			<td align="right" nowrap>
				<font color="#333333">Empenhado (R$)</font>
			</td>
			<? 
			$totalempenhado = 0;
			for( $j = 1; $j <= 12; $j++ ) { ?>
				<td align="right" style="color:#333333; font-size: 9px" onMouseOver="this.bgColor='#ffffcc';" onMouseOut="this.bgColor='';">
				<?= number_format( $dadofinanceiropormes[sprintf("%02d",$j)]['empenhado'], 0, ',', '.'); $totalempenhado += $dadofinanceiropormes[sprintf("%02d",$j)]['empenhado']; ?>
				</td>
			<? } ?>
			<td align="right" style="color:#000000; font-size: 9px">
				<?= number_format( $totalempenhado, 0, ',', '.'); ?>
			</td>
			<td align="right" style="color:#000000; font-size: 9px">
				<?= number_format( $dadofinanceiropormes['totalautorizado'], 0, ',', '.'); ?>
			</td>
			<td align="right" style="color:#000000; font-size: 9px">
				<?php if( $dadofinanceiropormes['totalautorizado'] != 0 ) : ?>
					<?= number_format( ($totalempenhado * 100 ) / $dadofinanceiropormes['totalautorizado'] , 0, ',', '.'); ?> %
				<?php else : ?>
					-
				<?php endif; ?>
			</td>
		</tr>
		<tr style="height:30px;" bgcolor="#f1f1f1">
			<td align="right" nowrap>
				<font color="#333333">Liquidado (R$)</font>
			</td>
			<? 
			$totalliquidado = 0;
			for( $j = 1; $j <= 12; $j++ ) { ?>
				<td align="right" style="color:#333333; font-size: 9px" onMouseOver="this.bgColor='#ffffcc';" onMouseOut="this.bgColor='';">
					<?= number_format( $dadofinanceiropormes[sprintf("%02d",$j)]['liquidado'], 0, ',', '.'); $totalliquidado += $dadofinanceiropormes[sprintf("%02d",$j)]['liquidado'];?>
				</td>
			<? } ?>
			<td align="right" style="color:#000000; font-size: 9px">
				<?= number_format( $totalliquidado, 0, ',', '.'); ?>
			</td>
			<td align="right" style="color:#000000; font-size: 9px">
				<?= number_format( $dadofinanceiropormes['totalautorizado'], 0, ',', '.'); ?>
			</td>
			<td align="right" style="color:#000000; font-size: 9px">
				<?php if( $dadofinanceiropormes['totalautorizado'] != 0 ) : ?>
					<?= number_format( ( $totalliquidado * 100 ) / $dadofinanceiropormes['totalautorizado'] , 0, ',', '.'); ?> %
				<?php else : ?>
					-
				<?php endif; ?>
			</td>
		</tr>
		<tr style="height:30px" bgcolor="#f1f1f1">
			<td align="right" nowrap>
				<font color="#333333">Pago (R$)</font>
			</td>
			<? 
			$totalpago = 0;
			for( $j = 1; $j <= 12; $j++ ) { ?>
				<td align="right" style="color:#333333; font-size: 9px" onMouseOver="this.bgColor='#ffffcc';" onMouseOut="this.bgColor='';">
					<?= number_format( $dadofinanceiropormes[sprintf("%02d",$j)]['pago'], 0, ',', '.'); $totalpago += $dadofinanceiropormes[sprintf("%02d",$j)]['pago'];?>
				</td>
			<? } ?>
			<td align="right" style="color:#000000; font-size: 9px">
				<?= number_format( $totalpago, 0, ',', '.'); ?>
			</td>
			<td align="right" style="color:#000000; font-size: 9px">
				<?= number_format( $dadofinanceiropormes['totalautorizado'], 0, ',', '.'); ?>
			</td>
			<td align="right" style="color:#000000; font-size: 9px">
				<?php if( $dadofinanceiropormes['totalautorizado'] != 0 ) : ?>
					<?= number_format( ( $totalpago * 100 ) / $dadofinanceiropormes['totalautorizado'] , 0, ',', '.'); ?> %
				<?php else : ?>
					-
				<?php endif; ?>
			</td>
		</tr>
	</TBODY>
	<?php if( count( $meses ) > 0 ): ?>
	<tr>
		<td colspan="16" bgcolor="#ffffff" align="center">
			<p align="right"><i>* Dados financeiros atualizados até <? echo formata_data($dataatualizada); ?></i></p>
			<p style="padding: 10px"><a href="javascript:exibirGrafico();"><img src="../imagens/graph.gif" style="border: 0"/> Gráfico</a></p>
			<div id="grafico" style="display: none">
				<img src="../geral/grafico_acao.php?acaid=<?= $_SESSION['acaid'] ?>"/>
			</div>
		</td>
	</tr>
	<tr><td colspan="16" style="height:1px;background-color:#e5e5e5;padding:0px"></td></tr>
	<tr><td colspan="16" style="height:2px;background-color:#000000;padding:0px"></td></tr>
	</table>
	
	<script>
		function exibirGrafico(){
			elemento = document.getElementById( 'grafico' );
			if ( elemento.style.display == 'block' ) {
				elemento.style.display = 'none';
			} else {
				elemento.style.display = 'block';
			}
		}
	</script>
	<?php endif;
	?>
<br>
  <?
    $sql = "select ref.*,av.tpscod as avtps,av.corcod as avcor, av.avpid as avaid, av.tpaid as tpav, av.avptexto as avaliacao, ";
	$sql = $sql." av.avpliberada as avlib,av.usucpf as avusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as avdata, ca.corimgav, ca.corsignificado as avcordsc, ca.cordsc as avcornome,  tsa.tpsdsc as avtpsdsc, pa.tpscod as partps, tsp.tpsdsc as partpsdsc ,pa.corcod as parcor, ";
	$sql = $sql." pa.avpid as parid, pa.avpliberada as parlib, pa.tpaid as tppar, pa.avptexto as parecer,pa.usucpf as parusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as pardata, ";
	$sql = $sql." cp.corimgpar, cp.corsignificado as pacordsc, cp.cordsc as parcornome, tsa.tpsdsc as patpsdsc, exp.exprealizado ";
	$sql = $sql." from referencia ref ";
	$sql = $sql." left join avaliacaoparecer av on av.refcod=ref.refcod and av.tpaid=1 and av.acaid = ".$_SESSION['acaid'];
	$sql = $sql." left join cor ca on av.corcod = ca.corcod ";
	$sql = $sql." left join tiposituacao tsa on tsa.tpscod = av.tpscod ";
	$sql = $sql." left join avaliacaoparecer pa on pa.refcod=ref.refcod and pa.tpaid=2 and pa.acaid = ".$_SESSION['acaid'];
	$sql = $sql." left join cor cp on pa.corcod = cp.corcod ";
	$sql = $sql." left join tiposituacao tsp on tsp.tpscod = pa.tpscod";
	$sql = $sql." left join execucaopto exp on ".$_SESSION['acaid']." = exp.acaid and ref.refcod = exp.refcod ";
	$sql = $sql." where ref.refdata_limite_avaliacao_aca is not null";
	
	if ($_REQUEST['refcod'] and $_REQUEST['refcod'] <> 'x')
        $sql = $sql." and ref.refsnmonitoramento='t' and ref.refano_ref='".$_SESSION['exercicio']."' and ref.refcod=".$_REQUEST['refcod']." order by refano_ref desc,refmes_ref desc ";
else if ($_REQUEST['refcod'])
		$sql = $sql." and ref.refsnmonitoramento='t' and ref.refano_ref='".$_SESSION['exercicio']."' order by refano_ref desc,refmes_ref desc ";
//$sql .= " limit 1";

  $RS = $db->record_set($sql);

  $nlinhas = $db->conta_linhas($RS);
  // com o acaid e o usucpf da sessão posso saber se o usuário terá ou não acesso à edição dos registros.
  // verifica se é coordenador de ação --- alteração temporaria BGU
  if ($db->testa_coordenador($_SESSION['acaid'],'A') )
  {
  		$autorizado_av = true;
  }
  else if ( $acasnbgu == 't' )
  {
 	$autorizado_av = false;
  }
  
  
  //if ($db->testa_coordenador($_SESSION['acaid'],'A')) $autorizado_av = true;
  // verifica se é digitador
  if ($db->testa_digitador($_SESSION['acaid'],'A')) $autorizado_dig = true; 
  // caso a autorização não tenha sido feita, testo se é super usuário
  if ($db->testa_superuser() ) {
     $autorizado_av = true;
  }
//dbg( $sql );

//}
?>
<head>
<style type="text/css">
h7 {font-size: 90%}
</style>
</head>

<table border="0" cellspacing="0" cellpadding="0" align="center" bgcolor="#F5F5F5" class="tabela">
  <tbody>
   <?
   $mensagem = '<h7><font color="red"><b>&nbsp;ATENÇÃO Sr.(a) COORDENADOR (A), não esqueça de pressionar o botão LIBERAR<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;para que a avaliação seja válida para o MEC!</h7></b>';
   
   if ($_REQUEST['refcod'])
   {
	  for ($i=0;$i<=$nlinhas;$i++)
	  {
	    $habil= 'N';
	   $res =  $db->carrega_registro($RS,$i);
	   
	   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	   
	 //  dump( $res);
	   
	   
	   if ($avlib<>'t') $avcornome="";
	   if ($parlib<>'t') $parcornome="";
	   if (trim($parcornome)=='') $parcornome="_";
		?>
		   <tr align="center" bgcolor="#DCDCDC">
		   	<td colspan="2" style="BORDER-TOP: #000000 2px solid; BORDER-BOTTOM: #cccccc 1px solid; color:#006699; font-size: 10pt; padding: 3px;" align="left"><img align="top" src="../imagens/Sin_<?=trim($avcornome).'_'.trim($parcornome)?>.gif" border="0"> <strong><h7>Período de Referência: <?=$refdsc?></strong>
			   <?
			   //apenas para mostrar a mensagem...
			    if (($autorizado_av or $autorizado_dig))
			    {
			      // se for o titular e  ainda está dentro do prazo, pode liberar ou bloquear para nova edição
			      if ($autorizado_av)
			      {
				 if ((!$avlib or $avlib=='f') and $avaid > 0)
				      echo $mensagem ;
			      }
			    }
			   ?>
		   	</td>
		   </tr>
		   <tr><td width="50%" bgcolor="#e9e9e9" align="center" style="BORDER-RIGHT: #cccccc 1px solid;color:#990000;" valign="top">Avaliação</td><td width="50%" align="center" bgcolor="#e9e9e9" style="color:#990000;" valign="top">Acompanhamento Físico</td></tr>
		   <tr>
		   <td align="center" style="BORDER-RIGHT: #cccccc 1px solid;" valign="top" width="60%">
		<?   
	   // incia a avaliação *********************************************
	    include (APPRAIZ.$_SESSION['sisdiretorio']."/modulos/principal/acao/avalia_aca_old.inc");
	   // *******************************************************************//
	   //   divide avaliação de parecer
	  }
   }
  ?>
</TR>
</tbody>
<tr><td colspan='4' class="SubTituloDireita">&nbsp;<br>
&nbsp;</td></tr>
  </table>
  <table>
  
<?
    if (($autorizado_av or $autorizado_dig) and $habil=='S')
    {
      if ($_REQUEST['refcod']=='x') { // se for o titular e  ainda está dentro do prazo, pode liberar ou bloquear para nova edição
          ?> <tr><td>
               <input type="button" class="botao" name="btgravageral" value= "Gravar todas as avaliações."  onclick="grava_av('<?=$refcod?>',0,100000)">&nbsp;&nbsp;&nbsp;
      </td></tr>
      <?
      
      $mostraMensagem = true;
      
      }
    }

    ?>
</table>
  </center>
</div>
</form>
<script><!--
    function verregistro(cod) {
        document.formulario.navega.value = cod;
	document.formulario.submit();
    }
   
function grava_av(refcod,cod,i)
{
	avaliacao = document.getElementById('campoDesc').value;	
 
	//try {
    //     document.formulario.btgravageral.disabled=true;  
	//	} 
	//catch(e) { }    	
	
	
    document.formulario.refren.value=refcod;
    document.formulario.tipograva.value='A';
    document.formulario.doccod.value=cod;
    document.formulario.avptexto.value = i;
    
    /*********** Validações **************/
    var erro = 0;
	var valorCor = 0;
	var msn = '';
	var msn2 = '';
	var msn3 = '';
	//avaliacao = document.getElementById('campoDesc').value;	
	
	//if(escape(tinyMCE.getInstanceById(avaliacao).getHTML()) == ""){
	if( tinyMCE.get(avaliacao).getContent() == "" )
	{
		msn += "A Avaliação não pode estar em branco. \n";
		erro = 1;
	}

	var totalChecks  = 0;
	var totalChecked = 0;
	var totalCoresMarcadas = 0;

	for (cont=0;cont<document.formulario.elements.length;cont++){
		 //avaliacao = document.formulario.elements[cont].name.search(/avaliacao_/); 
		 periodo   = document.formulario.elements[cont].name.search(/exprealizado/); 
		 situacao  = document.formulario.elements[cont].name.search(/tpscodav/); 
		 if (document.formulario.elements[cont].type == "radio"){
		 	cor = document.formulario.elements[cont].name.search(/corcodav/);
		 	if (cor != -1) {
		 		totalChecks++;
		 		//alert(document.formulario.elements[cont].checked);
		 		if (document.formulario.elements[cont].checked == true){
		 			totalChecked++
		 			totalCoresMarcadas++;
			 	}

		 		if (document.formulario.elements[cont].checked == false){
		 			valorCor = valorCor+1;
		 		}
		 	}
		 }

		if(periodo != -1){
			if(document.formulario.elements[cont].value == ''){
				//msn +="Informe o Executado no Período. Campo Obrigatório \n";
				msn = "Informe o Executado no Período. Campo Obrigatório. \n";
				erro = 1;
			}
		}
		if(situacao != -1){
			if(document.formulario.elements[cont].value == ''){
				//msn +="Escolha uma situação. Campo obrigatório \n";
				msn2 ="Escolha uma situação. Campo Obrigatório. \n";
				erro = 1;
			}
		}
	}

	//Comentando por que não estava funcionando quando escolhia Todas as Referências	
	/*if ((totalChecks %  4 == 0 && totalChecked %  4 != 0) ||
		(totalChecks == 3      && totalChecked != 1))
	{
		//msn +="Escolha uma das cores. Campo Obrigatório \n";
		msn3 = "Escolha uma das cores. Campo Obrigatório. \n";
		erro = 1;
	}*/


	var aSerMarcadas = totalChecks/3;
	if(totalCoresMarcadas == 0 || totalCoresMarcadas < aSerMarcadas){
		msn3 = "Escolha uma das cores. Campo Obrigatório. \n";
		erro = 1;
	}
	
	if(msn != '' || msn3 != '' || msn3 != ''){
		alert(msn+msn2+msn3);
	}
    
    /********* Fim Validações ************/
    
    if (submited){
      	alert ('Aguarde o Processamento!');
    }else{
    	if(erro == 0){  
    	if(document.formulario.btgrava){
	    	document.formulario.btgrava.disabled=true;  
    	}
	    document.formulario.submit();
	    var submited = true;
	    }
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
    	document.formulario.boComboReferencia.value = 1;
      	document.formulario.submit();
 	}
     function edita_dadacao() {
      e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/editaracao&acao=A";
      WindowObjectReference = window.open(e, "editar_acao","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=650,height=350'");
    }

--></script>