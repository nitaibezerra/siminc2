<?
	include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
	include_once( APPRAIZ. "pse/classes/EstadoMunicipioPSE.class.inc" );
	include APPRAIZ . "includes/classes/dateTime.inc";
	
	// monta cabeçalho
	include APPRAIZ . 'includes/cabecalho.inc';
	/*if($pflcod == SECRETARIA_MUNICIPAL){
		$nmMunEst = 'm';
	} else {
		$nmMunEst = 'e';
	}*/
	
	$nmMunEst = $_SESSION['pse']['flagmun'];
	
	$obEstadomunicipiopse = new EstadoMunicipioPSE($_SESSION['pse']['empid'],$_SESSION['pse']["muncod"], $nmMunEst );
	
	$obEstadomunicipiopse->muncod = $_SESSION['pse']["muncod"];
	$obEstadomunicipiopse->empflagestmun = $nmMunEst;
	
	$muncod = $_SESSION['pse']["muncod"];
	$empid = $obEstadomunicipiopse->empid;
	$obEstadomunicipiopse->verEmp($empid);
	
	$titulo = "PSE - Programa Saúde na Escola";
	$titulo2 = "MATERIAIS CLÍNICOS E DIDÁTICOS";
	echo "<br>";
	monta_titulo( $titulo, '' );
	monta_titulo( $titulo2, '' );
	
	$sql = "SELECT perid,perflagboleana,perflagitem,perflagmultiescolha,perdescricao,pernumeroordem
			FROM pse.pergunta
			WHERE perid = 19";
	$dados = $db->pegaLinha($sql);
	
	$sql = "SELECT i.iteid
			FROM pse.itemselecionado i
			INNER JOIN
				pse.resposta r on i.resid=r.resid
			WHERE
				r.perid=19 and
				r.empid=$empid";
	$check = $db->pegaum($sql);
	

if($_POST){	
	$sql = "delete from pse.secrekitcompleto
			where empid =$empid";
	$db->executar($sql);
	
	$sql = "delete from pse.itemselecionado
			where resid = (SELECT i.resid FROM pse.itemselecionado i
							INNER JOIN
								pse.resposta r on i.resid=r.resid
							WHERE r.perid=19 and r.empid=$empid)";
	$db->executar($sql);

	$sql = "delete from pse.resposta
			where empid = $empid and perid=19";
	$db->executar($sql);
	
	$check = $_REQUEST['perg_19'];
	
	if (!empty($check)){
		//tabela resposta
		$sql = "INSERT INTO pse.resposta
					(perid,empid,resflagresposta)
					VALUES
					(19, $empid, 's')
				RETURNING resid";
		$resid = $db->pegaUm($sql);
		$db->commit();
		
		//tabela item selecionado
		$sql = "INSERT INTO pse.itemselecionado
					(iteid,resid)
				VALUES
					($check,$resid)";
		$db->executar($sql);
		$db->commit();
			
			
		if($check!=55){		
			$kitid = $_REQUEST['kitid'];
			$sql = "delete from pse.secrekitcompleto
					where empid = $empid";
			$db->executar($sql);
			$db->commit();
			foreach ($kitid as $chave) {
				$qesc = $_REQUEST['kitidescola_'.$chave];
				$qesf = $_REQUEST['kitidesf_'.$chave];
				$qesc = ($qesc==''?'null':$qesc);
				$qesf = ($qesf==''?'null':$qesf);
				/*if(!is_numeric($qesc) || !is_numeric($qesf)) {
					echo "<script>alert('Utilize somente números!');history.go(-1);</script>";
					exit;
				}*/
				
				if(is_numeric($qesc) && is_numeric($qesf)) {
					$sql = "INSERT INTO pse.secrekitcompleto
							(kitid,empid,skcquantescola,skcquantesf)
							VALUES
							($chave,$empid,$qesc,$qesf)";
					$db->executar($sql);
					$db->commit();
				}
				
			}
		}
		echo "<script>alert('Operação realizada com sucesso!');</script>";
	}
	else
		echo "<script>alert('A pergunta 4.1 é obrigatória!');</script>";
}
	
?>


<form method="POST" name="formulario" id="formulario" onsubmit="return Valida()">
<?php Cabecalho($muncod,2,$nmMunEst); ?>
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><?=$dados['pernumeroordem'].".  ". $dados['perdescricao'];?></td>
	</tr>
	<tr>
		<td style="text-align: left">
			<?php
				$sql ="SELECT
							iteid as codigo, 
							itedescricao as descricao
						FROM 
							pse.itempergunta
						WHERE
							perid=19";
				$radio = $db->carregar($sql);
			
				foreach($radio as $valor => $rotulo){
					if ($rotulo['codigo']==$check)
						$marca = "checked";
					else
						$marca = "";
					echo "<input type='radio' $marca name='perg_19' id='perg_19' value='$rotulo[codigo]' onclick='selecionaItens(this.value)'>$rotulo[descricao]";
				}
			?>

    	</td>
	</tr>
	<tr id="divitens">
		<td class="SubTituloEsquerda" colspan="2"><?=itensPergunta($empid);?></td>
	</tr>

	<?php if(($nmMunEst=='e' && $pflcod == SECRETARIA_ESTADUAL) || ($nmMunEst=='m' && $pflcod == SECRETARIA_MUNICIPAL) || $pflcod == SUPER_USUARIO){?>
		<?php 
		$sql = "SELECT * FROM pse.programacaoexercicio WHERE prsano = '".$_SESSION['exercicio']."'";
		$arr = $db->pegaLinha( $sql );
		$dataAtual = date("Y-m-d");
		$data = new Data();
		$dataF = trim($arr['prsdata_termino']);
		$resp = 1;
			if( !empty($dataF) ){
				$resp = $data->diferencaEntreDatas($dataAtual, $arr['prsdata_termino'], 'maiorDataBolean','','');		
			}
		if( $resp == NULL ){ ?>
			<tr style="background-color: #DCDCDC">
				<td colspan="2" align="center"><input type="submit" value="Salvar"></td>
			</tr>
		<?php } ?>
	<?php }?>
	<?=navegacao('pse2009','grupoGestor');?>
	</table>
</form>	

<?php
	$sql = "SELECT max(kitid)
			FROM pse.kitpedagoclinico";
	$quant = $db->pegaUm($sql);
?>

<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	function selecionaItens(vr){
		if(vr==55)
			$('divitens').hide();
		else
			$('divitens').show();
	
	}
			
	function Valida(){
	/*	
		if( document.getElementById('perg_19').checked = true  ){
			alert('A pergunta 4.1 é obrigatória!');
			return false;
		}
	*/	
	}

	
	function calculaTotalPedagogico(tp){
		somaPESC = parseInt(0);
		somaPESC = (somaPESC - Number(eval("document.formulario.totalCesc").value));
		somaPESF = parseInt(0);
		somaPESF = (somaPESF - Number(eval("document.formulario.totalCesf").value));

		nrobj = <?=($quant*4);?>;

		for (i=1;i<=nrobj;i++){
	      if(document.formulario.elements[i].type == "text"){
	      	campo = document.formulario.elements[i].id;
	      	for (ii=0;ii<=nrobj;ii++){ 
	              if(campo == 'kitidescola_'+ii){
	              	vrESC = Number(eval("document.formulario."+campo).value);
              		somaPESC = (somaPESC + vrESC);
                	document.getElementById("totalpedagogicoEscola").innerHTML = "<b>"+somaPESC+"</b>";
	              }
	              else if(campo == 'kitidesf_'+ii){
	              	vrESF = Number(eval("document.formulario."+campo).value);
              		somaPESF = (somaPESF + vrESF);
                	document.getElementById("totalpedagogicoESF").innerHTML = "<b>"+somaPESF+"</b>";
	              }
	         }
	       }
		}
	}
	
	function calculaTotalClinico(tp){
		somaCESC = parseInt(0);
		somaCESC = (somaCESC - Number(eval("document.formulario.totalPesc").value));
		somaCESF = parseInt(0);
		somaCESF = (somaCESF - Number(eval("document.formulario.totalPesf").value));
		
		nrobj = <?=($quant*4);?>;

		for (i=1;i<=nrobj;i++){
		
		if(tp=='c'){
	      if(document.formulario.elements[i].type == "text"){
	      	campo = document.formulario.elements[i].id;
	      	for (ii=0;ii<=nrobj;ii++){ 
	              if(campo == 'kitidescola_'+ii){
	              	vrESC = Number(eval("document.formulario."+campo).value);
              		somaCESC = (somaCESC + vrESC);
                	document.getElementById("totalclinicoescola").innerHTML = "<b>"+somaCESC+"</b>";
	              }
	              else if(campo == 'kitidesf_'+ii){
	              	vrESF = Number(eval("document.formulario."+campo).value);
              		somaCESF = (somaCESF + vrESF);
                	document.getElementById("totalclinicoesf").innerHTML = "<b>"+somaCESF+"</b>";
	              }
	         }
	       }
		}
		}
	}

	selecionaItens(<?=$check;?>);

</script>