<?

	include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
	VerSessao();	
	include APPRAIZ . 'includes/cabecalho.inc';

	$entid = $_SESSION['pse']["entid"];
	$espid = $_SESSION['pse']["espid"];
	if( $entid == '' || empty($entid)){
		print "<script>"
			. "    alert('Faltam dados para esta tela!');"
			. "    history.back(-1);"
			. "</script>";
		
		die;
	}
	$perguntaPage = "10,11";
	$ano = 1;
	
	$titulo = "PSE - Programa Saúde na Escola";
	$titulo2 = "Unidade Local Integrada - 2 - Necessidades e Carências em Saúde - Componente I";
	echo "<br>";
	monta_titulo( $titulo, '' );
	monta_titulo( $titulo2, '' );
	
	$sql = "SELECT iulid 
			FROM pse.iulitempergunta
			WHERE pulid = 10 and iuldescricao = 'Não'";
	$nao = $db->pegaUm($sql);
	
	if($_POST){
		$idpergunta = explode(",",$_REQUEST['rulid']);
		
	/*	$sql = "SELECT iulid 
				FROM pse.iulitempergunta
				WHERE pulid = 10 and iuldescricao = 'Não'";
		$nao = $db->pegaUm($sql);*/
		
		foreach($idpergunta as $pergunta){
			$check = $_REQUEST['perg_10'];
			if($check==''){
				echo "<script>alert('A pergunta 2.1 é obrigatória!');history.go(-1);</script>";
				exit;
			}
			$sql = "SELECT rulid FROM pse.rulresposta
					WHERE pulid = $pergunta and espid = $espid";
			$rulid = $db->pegaUm($sql);

			if($rulid==''){
				$sql = "INSERT INTO pse.rulresposta
						(pulid,espid,paqid,rulflagresposta)
						VALUES
						($pergunta,$espid,$ano,'s')
						RETURNING rulid";
				$rulid = $db->pegaUm($sql);
			}

			//Deletando dados
			
			$sql = "DELETE FROM pse.tultextoitemselecionado
					WHERE isuid in (select isuid from pse.isuitemselecionado where rulid = $rulid)";
			$db->executar($sql);
			
			$sql = "DELETE FROM pse.isuitemselecionado WHERE rulid = $rulid";
			$db->executar($sql);
			
			$db->commit();
			
			if($pergunta==10){
				$sql = "INSERT INTO pse.isuitemselecionado
							(iulid,rulid)
							VALUES
							($check,$rulid)";
				$db->executar($sql);
				$db->commit();
			}
			else {
				$sql ="SELECT i.iulid as codigo, i.iuldescricao
						FROM pse.iulitempergunta i
						WHERE i.pulid=$pergunta";
				$dados = $db->carregar($sql);
				
				if($check<>$nao){
						foreach($dados as $itens){
							$item = $itens['codigo'];
							
							$vrEscola	= ($_REQUEST['tulquantescola_'.$item]==''? 'null' : $_REQUEST['tulquantescola_'.$item]);
							$vrESF		= ($_REQUEST['tulquantesf_'.$item]==''? 'null' : $_REQUEST['tulquantesf_'.$item]);

							if($vrEscola=='null' && $vrESF=='null'){
								//alert('vazio');
							}
							else {
								if(!is_numeric($vrEscola)||!is_numeric($vrESF)){
									alert('Utilize somente números na quantidade!');
									
								}
								else {
								$sql = "INSERT INTO pse.isuitemselecionado
										(iulid,rulid)
										VALUES
										($item,$rulid)
									RETURNING isuid";
								$isuid = $db->pegaUm($sql);
									
								$sql = "INSERT INTO pse.tultextoitemselecionado
										(isuid,tulquantescola,tulquantesf)
										VALUES
										($isuid,$vrEscola,$vrESF)";
								$db->executar($sql);
								}
							}
						}
						$sql = "SELECT count(*)
										FROM pse.isuitemselecionado i
										INNER JOIN
											pse.tultextoitemselecionado t ON t.isuid = i.isuid
										WHERE i.rulid=$rulid";
						$acao = $db->pegaUm($sql);
						if($acao>0)
							$db->commit();
						else {
							$db->rollback();
							alert('Informe as quantidades de materiais!');
						}
				}
				
			}
		}
			
	}

	
$sql = "SELECT iulid 
		FROM pse.iulitempergunta
		WHERE pulid = 11";
$dados = $db->carregar($sql);
$arrPedagogico	= array();
$arrClinico		= array();
$i = 1;
foreach($dados as $dados){
	if($i <= 5)
		$arrPedagogico[$i] = $dados['iulid'];
	else
		$arrClinico[$i] = $dados['iulid'];
	
	$i++;
}
	
$sql = "SELECT iulid 
		FROM pse.isuitemselecionado i
		INNER JOIN pse.rulresposta r ON r.rulid=i.rulid
		WHERE r.pulid = 10 AND espid = $espid";
$checa = $db->pegaUm($sql);

$sql = "SELECT  '1' as id, sum(tulquantescola) as totesc,sum(tulquantesf) as totesf
		from pse.tultextoitemselecionado t
		inner join
		pse.isuitemselecionado i on i.isuid=t.isuid 
		INNER JOIN 
		pse.rulresposta r ON r.rulid=i.rulid
		where r.pulid = 11 AND r.espid = $espid and i.iulid in (".implode(",",$arrPedagogico).")
		union all
		SELECT  '2' as id, sum(tulquantescola) as totesc,sum(tulquantesf) as totesf
		from pse.tultextoitemselecionado t
		inner join
		pse.isuitemselecionado i on i.isuid=t.isuid 
		INNER JOIN 
		pse.rulresposta r ON r.rulid=i.rulid
		where r.pulid = 11 AND r.espid = $espid and i.iulid in (".implode(",",$arrClinico).")";
$totais = $db->carregar($sql);

foreach($totais as $total){
	if($total['id']==1){
		$totalesc1	= ($total['totesc']==''?'':$total['totesc']);
		$totalesf1	= ($total['totesf']==''?'':$total['totesf']);
	}
	else {
		$totalesc2	= ($total['totesc']==''?'':$total['totesc']);
		$totalesf2	= ($total['totesf']==''?'':$total['totesf']);
	}
}

?>

<script language="JavaScript" src="../includes/funcoes.js"></script>

<form method="POST"  name="formulario" onsubmit='return verifica()'>
<?php Cabecalho($entid,1); ?> 
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><input type='hidden' name='rulid' id='rulid' value='<?=$perguntaPage;?>'></td>
	</tr>
	
	<?php 
		$idpergunta = explode(",",$perguntaPage);
			
		foreach($idpergunta as $pergunta){
			$sql ="SELECT puldescricao
					FROM pse.pulpergunta
					WHERE pulid=$pergunta";
			$descPergunta = $db->pegaUm($sql);
			
			if( $pergunta == 11 ){
				$where2 = "";
				
				
				$sql = "SELECT
							pp.porano + 1
						FROM
							pse.escolapse ep
						INNER JOIN
							entidade.entidade ent ON ent.entid = ep.entid				
						INNER JOIN
							entidade.endereco ende ON ende.entid = ent.entid
						INNER JOIN
							territorios.municipio m ON m.muncod = ende.muncod
						INNER JOIN
							pse.portariapse pp ON pp.pormunicipio = m.muncod
						WHERE
							ep.entid = ".$entid;
				
				$ano = $db->pegaum( $sql );
				
				if( !($ano == $_SESSION['exercicio']) ){
					$where2 = "AND iulano = ".$_SESSION['exercicio'];
				} else {
					$where2 = "AND iulano <= ".$_SESSION['exercicio'];
				}
			}
			
			$sql ="SELECT i.iulid, i.iuldescricao, i.iulflagvalor
					FROM pse.iulitempergunta i
					WHERE i.pulid={$pergunta} {$where2}";

			$dados = $db->carregar($sql);
			echo "<tr>
						<td style='text-align: left' class='SubTituloEsquerda' colspan='2'><b>"
						 	.$descPergunta."
						 </b></td>
				   </tr>";
			if($pergunta==11){
				echo "<tr>
						<td style='text-align: left' colspan='2'>
							<table id='itens11' width='100%' style='border-bottom: 0px;' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3'>
							<tr>
								<td align='center' style='background: rgb(238, 238, 238)'><b>Materiais</b></td>
								<td align='center' style='background: rgb(238, 238, 238)' width='10%'><b>Escolas</b></td>
								<td align='center' style='background: rgb(238, 238, 238)' width='10%'><b>ESF</b></td>
							</tr>";
			}
			if( is_array($dados) ){	   
				foreach($dados as $item){
					$sql = "	SELECT i.iulid, t.tulquantesf, t.tulquantescola
								FROM pse.isuitemselecionado i
								INNER JOIN
									pse.rulresposta r ON r.rulid = i.rulid
								LEFT JOIN
									pse.tultextoitemselecionado t ON t.isuid = i.isuid
								WHERE i.iulid = $item[iulid] and r.pulid = $pergunta and r.espid=$espid ";
					$valores = $db->pegaLinha($sql);
					
					$vrEscola	= ($valores['tulquantescola']==''?'':$valores['tulquantescola']);
					$vrESF		= ($valores['tulquantesf']==''?'':$valores['tulquantesf']);
	
					if($item['iulflagvalor']=='n'){
						
						if($valores['iulid']==$item['iulid'])
							$checado = "checked";
						else
							$checado = "";
							
						echo "<tr>
								<td style='text-align: left' colspan='2'>
								 	<input type='radio' name='perg_10' id='perg_10' $checado value='".$item['iulid']."' onclick='check(".$item['iulid'].")'> ".$item['iuldescricao'].
								 "</td>
							   </tr>";
					}
					else {
						echo " <tr>
								<td style='text-align: left'>
										".$item['iuldescricao']."
								</td>
								<td style='text-align: center' width='10%'>
									<input class='normal' maxlength='6' type='text' value='".$vrEscola."' name='tulquantescola_".$item['iulid']."' id='tulquantescola_".$item['iulid']."' size='10' style='text-align:center' onkeypress='return somenteNumeros(event);' onKeyUp='calcula(this.value,".$item['iulid'].")'>
								
								<td style='text-align: center' width='10%'>
									<input class='normal' maxlength='6' type='text' value='".$vrESF."' name='tulquantesf_".$item['iulid']."' id='tulquantesf_".$item['iulid']."' size='10' style='text-align:center' onkeypress='return somenteNumeros(event);' onKeyUp='calcula(this.value,".$item['iulid'].")'>
								</td>
							   </tr>";
						if($item['iulid'] == end($arrPedagogico)){
							echo "	
								<tr>
									<td class='SubTituloEsquerda' style='background: rgb(238, 238, 238)'>Total Materiais Pedagógicos</td>
									<td align='center' style='background: rgb(238, 238, 238)'><input class='normal' maxlength='6' type='hidden' value='".($totalesc1==''?0:$totalesc1)."' name='totalesc1' id='totalesc' size='10' style='text-align:center'><div id='totalesc1'><b>".$totalesc1."</b></div></td>
									<td align='center' style='background: rgb(238, 238, 238)'><input class='normal' maxlength='6' type='hidden' value='".($totalesf1==''?0:$totalesf1)."' name='totalesf1' id='totalesf' size='10' style='text-align:center'><div id='totalesf1'><b>".$totalesf1."</b></div></td>
								</tr>";
						}	   
							   
					}
				}
			}
			if($pergunta==11){
				echo "<tr>
							<td class='SubTituloEsquerda' style='background: rgb(238, 238, 238)'>Total Materiais Clínicos</td>
							<td align='center' style='background: rgb(238, 238, 238)'><input class='normal' maxlength='6' type='hidden' value='".($totalesc2==''?0:$totalesc2)."' name='totalesc2' id='totalPesc' size='10' style='text-align:center'><div id='totalesc2'><b>".$totalesc2."</b></div></td>
							<td align='center' style='background: rgb(238, 238, 238)'><input class='normal' maxlength='6' type='hidden' value='".($totalesf2==''?0:$totalesf2)."' name='totalesf2' id='totalPesf' size='10' style='text-align:center'><div id='totalesf2'><b>".$totalesf2."</b></div></td>
						</tr></table></td></tr>";
			}
			
			
		}
	?>
	<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
		<tr style="background-color: #DCDCDC">
			<td colspan="2" align="center"><input type="submit" value="Salvar"></td>
		</tr>
	<?php }?>
	<?=navegacao('gestao05','compInecessidadeCarencia02');?>
	</table>
</form>	


<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	function  check(opcao){
		if(opcao=='<?=$nao;?>'){
			$('itens11').hide();
		}
		else
			$('itens11').show();
	}
	
	function calcula(valor,item){
		soma1esc = 0;
		soma1esf = 0;
		soma2esc = 0;
		soma2esf = 0;
		for(i=<?=$arrPedagogico[1];?>;i<=<?=end($arrPedagogico);?>;i++){
			soma1esc = soma1esc + Number(eval("document.formulario.tulquantescola_"+i).value);
			soma1esf = soma1esf + Number(eval("document.formulario.tulquantesf_"+i).value);
		}
		document.getElementById("totalesc1").innerHTML = "<b>"+soma1esc+"</b>";
		document.getElementById("totalesf1").innerHTML = "<b>"+soma1esf+"</b>";
		
		for(i=<?=$arrClinico[6];?>;i<=<?=end($arrClinico);?>;i++){
			soma2esc = soma2esc + Number(eval("document.formulario.tulquantescola_"+i).value);
			soma2esf = soma2esf + Number(eval("document.formulario.tulquantesf_"+i).value);
		}
		document.getElementById("totalesc2").innerHTML = "<b>"+soma2esc+"</b>";
		document.getElementById("totalesf2").innerHTML = "<b>"+soma2esf+"</b>";

	}

	check(<?=$checa;?>);
</script>