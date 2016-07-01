<? 
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Alexandre Soares Diniz
   Programador: Alexandre Soares Diniz
   Módulo:celulas.inc
   Finalidade: permitir o cadastro dos celulas
   */
//print_r($_REQUEST);


########### AÇÕES associação de categorias ###########
$modulo=$_REQUEST['modulo'] ;
if ($_REQUEST['act2']=='associar' and $_POST['pai']<>'') {
	
	//pega o nome dos combos
	foreach($_POST as $key => $value) { #Onde $key é o nome do campo (Nome)                                                   #e $value é o valor 
	//do campo $_POST['nome'] 
	  if(substr($key,0,5)=="combo") $campocombo .= "&$key=$value"; 
	} 
	$param = $campocombo . "&tcbid=".$_GET['tcbid'];

	  
  if($_POST['cdcidorigem']){
  
  	 $sql = "delete from public.combodadoscombinacao where cdcidpai={$_POST['filho']} and cdcidorigem = {$_POST['cdcidorigem']}";
  	 $db->executar($sql);
  
	  $cdcid = $_POST['cdcid'];
	  //print_r($_POST['cdcid']);
	  $nlinhas = count($cdcid)-1;
	
		for ($j=0; $j<=$nlinhas;$j++)
		{
		 $sql = "insert into public.combodadoscombinacao (cdcidpai, cdcidfilho, cdcidorigem) 
		  		  VALUES (".$_POST['filho'].",".$_POST['cdcid'][$j].",".$_POST['cdcidorigem'].")
		  		  ";
		 // print $_POST['cdcidfilho'][$j].'<br>';
		 $db->executar($sql);
		 //$cdcid = $db->pegaUm($sql);
		}
	  $db -> commit();
	  $db->sucesso($modulo, $param);
  	 
  }
  else{
  	/*
	  $cdcid = $_POST['cdcid'];
	  //print_r($_POST['cdcid']);
	  $nlinhas = count($cdcid)-1;
	
		for ($j=0; $j<=$nlinhas;$j++)
		{
		 $sql = "insert into public.combodadoscombinacao (cdcidpai, cdcidfilho) 
		  		  VALUES (".$_POST['pai'].",".$_POST['cdcid'][$j].")
		  		  RETURNING cdcid";
		 // print $_POST['cdcidfilho'][$j].'<br>';
		 //$db->executar($sql);
		 $cdcidnovo = $db->pegaUm($sql);
		 
		 $sql = "insert into public.combodadoscombinacao (cdcidpai, cdcidfilho) 
		  		  VALUES (".$_POST['pai'].",".$_POST['cdcid'][$j].")
		  		  RETURNING cdcid";
		 // print $_POST['cdcidfilho'][$j].'<br>';
		 //$db->executar($sql);
		 $cdcid = $db->pegaUm($sql);		 
		}
	  $db -> commit();
	  $db->sucesso($modulo);
  
     $sql = 'delete from public.combodadoscombinacao where cdcidpai='.$_POST['pai'];
     $db->executar($sql);
	*/

     
	 //deleta os filhos
	 $sql = "delete from public.combodadoscombinacao where cdcidorigem in (select  cdcid from combodadoscombinacao where cdcidpai = {$_POST['pai']})";
     $db->executar($sql);
		
     //deleta o pai
     $sql = 'delete from public.combodadoscombinacao where cdcidpai='.$_POST['pai'];
     $db->executar($sql);
     
     
	  $cdcid = $_POST['cdcid'];
	  //print_r($_POST['cdcid']);
	  $nlinhas = count($cdcid)-1;
	
		for ($j=0; $j<=$nlinhas;$j++)
		{
			 $sql = "insert into public.combodadoscombinacao (cdcidpai, cdcidfilho) 
			  		  VALUES (".$_POST['pai'].",".$_POST['cdcid'][$j].")
			  		  ";
			 // print $_POST['cdcidfilho'][$j].'<br>';
			 $db->executar($sql);
		}
	  $db -> commit();
	  $db->sucesso($modulo, $param);
     
     
  
  }

}


include APPRAIZ."includes/cabecalho.inc";
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Associação de Categorias';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<div align="center">
<center>

<?
//atribui valores as variaveis
$modulo=$_REQUEST['modulo'] ;//
//$ctbid = md5_decrypt($_REQUEST['sisidEdit'],'');
$cdtid = $_REQUEST['cdtid'];
$ctbid = $_REQUEST['ctbid'];
$tcbid = $_REQUEST['tcbid'];

//flag fixo
if($_REQUEST['combo5']) $pai = $_REQUEST['combo5']; 
if($_REQUEST['combo3']) $pai = $_REQUEST['combo3'];
if($_REQUEST['combo1']) $pai = $_REQUEST['combo1'];

if($_REQUEST['combo6']) $filho = $_REQUEST['combo6'];

if($pai) $cdtid = $pai;
if($filho) $cdtid = $filho;
//fim flag fixo

if ($cdtid):
	$sql= "SELECT
			 c.cdtid as cdtid,
			 c.ctbid as ctbid,
			 c.cdtcod as cod,
			 c.cdtdsc as dsc,
			 c.cdtstatus AS status,
			 cb.ctbidfilho as ctbidfilho
		   FROM
		     public.combodadostabela c
		     inner join public.COMBOTABELA cb on cb.ctbid = c.ctbid
		   WHERE 
		     c.cdtid = ".$cdtid."
		   ";
	$dados = (array) $db->carregar($sql);
	
	#### Transforma $dados[0] em Variáveis ####
	extract($dados[0]);
	
else:
	$cdtid 		   = "";
	//$ctbid 	   = "";
	//$ctbidfilho  = "";
	$cod		   = "";
	$dsc   		   = "";
	$status        = "A";
endif;

$act = '';
	
?>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="cdtidEdit" value="<?=$cdtid?>">
<input type='hidden' name="act" value=<?=$act?>>


    <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="SubTituloDireita">Tipo:</td>
	     <td>
	     	<?
	     		$sql = "SELECT tcbid as CODIGO, tcbdsc as DESCRICAO  FROM tipocombo;";
	     		$db->monta_combo("tcbid",$sql,'S',"-- Selecione o Tipo --",'filtratipo','');
	     		echo obrigatorio();
	     	?>
	     </td>
      </tr>
      <?
      if($tcbid){
	      $sql = "SELECT ctbid, ctbdsc FROM COMBOTABELA where tcbid = ".$tcbid. "order by ctbcampo";
		  $combos = $db->carregar($sql);

		  $i = 1;
	      foreach($combos as $combos2){

	       	if(count($combos) != $i){
		      	echo "<tr><td align='right' class='SubTituloDireita'>".$combos2['ctbdsc']."</td>";
		      	echo "<td>";
		      	
							      	
					$sql= "select 
					cdtid as codigo,
					cdtcod || ' - ' || cdtdsc as descricao
			 		from public.combodadostabela 
			 		Where ctbid = ".$combos2['ctbid']."
			 		order by 2 ";	      	
		      		$db->monta_combo("combo".$combos2['ctbid'],$sql,'S',"-- Selecione --",'filtracombo','','','','','','',$_REQUEST['combo'.$combos2['ctbid']]);
		      	
		      	echo "</td></tr>";
	      	}
	      	
	      	$i++;
	      }
      }      
      ?>
      
    </table>

  




<!-- LISTA ASSOCIAÇÃO -->


<br><br>

<input type='hidden' name="cdcidpai" value="<?=$cdtid?>">
<input type='hidden' name="pai" value="<?=$pai?>">
<input type='hidden' name="filho" value="<?=$filho?>">
<input type=hidden name="act2" value=0>

<?
 if ($cdtid) {
	?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr><td ALIGN=CENTER bgcolor="#DCDCDC">
			<B>Associação de Categorias: 
				<font color=blue>
					<?
					if($pai){
						$sql = "select cdtdsc from combodadostabela where cdtid=".$pai;
						echo $db->PegaUm($sql);
					}
					if($filho){
						echo " => ";
						$sql = "select cdtdsc from combodadostabela where cdtid=".$filho;
						echo $db->PegaUm($sql);
					}
					
					?>
				</font>
			</B>
		</td>
	</tr>
	</table>
	
 	<?
	if ($ctbidfilho){
		if($filho){
			
			if(!$pai){
				$alerta = "<script>
									alert('Selecione a Combo Anterior'); 
									location.href='monitora.php?modulo=sistema/tabapoio/associa_categoria&acao=A&tcbid={$tcbid}';
						   </script>";
				echo $alerta;
				exit;
			}
			
			$sql = "SELECT cdcid FROM combodadoscombinacao where cdcidfilho={$filho} and cdcidpai={$pai}";
			$cdcidorigem = $db->PegaUm($sql);
			echo "<input type='hidden' name='cdcidorigem' value='".$cdcidorigem."'>";
			
			$sql= "select cdcidfilho from combodadoscombinacao where cdcidpai = {$filho} 
				   and cdcidorigem in (SELECT cdcid FROM combodadoscombinacao where cdcidfilho={$filho} and cdcidpai={$pai} )
				   ";		
		}
		else{
			$sql= "select distinct cdcidfilho
				from combodadoscombinacao 
				where cdcidpai = {$cdtid}
				";
			
			$funcaocritica = "critica(this.value);";
		}
		
		//print $sql;
		$filhos = (array) $db->carregar($sql);
	
		foreach($filhos as $filhos2){
			$arrRows[] = "".$filhos2['cdcidfilho']."";
		}
		
		$filtrofilhos = implode(",",$arrRows);
		if(!$filtrofilhos) $filtrofilhos = 0;
		
		$sql= "select distinct
				case when c.cdtid in ({$filtrofilhos}) then '<input type=\"Checkbox\" name=\"cdcid[]\" value=\"' ||c.cdtid|| ' \" checked onclick=\"{$funcaocritica}\">' else '<input type=\"Checkbox\" name=\"cdcid[]\" value=\"'||c.cdtid||' \">' end as acao,
				--c.cdtid || ' ' as codigo,
				c.cdtcod || ' - ' || c.cdtdsc as descricao
				from public.combodadostabela c
				left join public.combodadoscombinacao cc ON (cc.cdcidpai = c.cdtid)  
				Where c.ctbid = ".$ctbidfilho."  	
				order by 2";
		//print $sql;
	//$cabecalho = array('<div align=left><input type=checkbox name=checatodos onclick=checkall()> Sim/Não</div>','Código','<div align=left>Nome</div>');
	$cabecalho = array('<div align=left><input type=checkbox name=checatodos onclick=checkall()> Sim/Não</div>','<div align=left>Nome</div>');
	$db->monta_lista_simples($sql,$cabecalho,300,20,'','','');
	
	}
	
	?>

	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr  bgcolor="#CCCCCC"><td colspan=2><input type='button' value='Associar' onclick="validar_associa()"></td></tr>
	</table>

<?}?>

</form>

<script language="JavaScript">

  function validar_associa() {

    document.formulario.act2.value = 'associar';
	document.formulario.submit();
  }
  
  function filtra(ctbid)
  {
  	location.href="monitora.php?modulo=sistema/tabapoio/categorias&acao=A&ctbid="+ctbid;
  }

  function filtratipo(tcbid)
  {
  	location.href="monitora.php?modulo=sistema/tabapoio/associa_categoria&acao=A&tcbid="+tcbid;
  }

  function filtracombo(cdtid)
  {
  	//tcbid = "<?=$tcbid?>";
  	//location.href="monitora.php?modulo=sistema/tabapoio/associa_categoria&acao=A&cdtid="+cdtid+"&tcbid="+tcbid;
  	document.formulario.submit();
  }
  
  function critica(cod){

	var form = document.getElementsByTagName("input");
	
	var campo = "";
	
	for(i=0; i<form.length; i++) {
		if( form[i].type == "checkbox")
		{
			if(form[i].value == cod){
				campo = form[i];
			}
		}
	}
	  
	if(campo.checked == false){
	  	if(confirm("A deseleção deste item e após clicar no botão 'Associar' irá apagar todas as associações referente o mesmo. \nDeseja realmente deselecionar?"))
	  	{
			campo.checked = false;
	  	}
	  	else{
	  		campo.checked = true;
	  	}
    }
  	
  }



function checkall(){
	
	var checado = document.formulario.checatodos.checked;
	var form = document.getElementsByTagName("input");
	
	if(checado == true)
	{
	  	for (var i=0;i<form.length;i++) {
			if( form[i].type == "checkbox")	form[i].checked = true;
		}
	}
	else
	{
	  	<?if(!$filho){?>
		  	if(confirm("A deseleção destes itens e após clicar no botão 'Associar' irá apagar todas as associações referente os mesmos. \nDeseja realmente deseleciona-los?"))
		  	{
			  	for (var i=0;i<form.length;i++) {
					if( form[i].type == "checkbox")	form[i].checked = false;
				}
		  	}
	  	<?}else{?>
		  	for (var i=0;i<form.length;i++) {
				if( form[i].type == "checkbox")	form[i].checked = false;
			}
		<?}?>
	}
	
}
  

</script>





