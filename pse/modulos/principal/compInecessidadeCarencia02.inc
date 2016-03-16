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
	$perguntaPage = "17,12";
	$ano = 1;
	
	$titulo = "PSE - Programa Saúde na Escola";
	$titulo2 = "Unidade Local Integrada - 2 - Necessidades e Carências em Saúde - Componente I";
	echo "<br>";
	monta_titulo( $titulo, '' );
	monta_titulo( $titulo2, '' );

	if($_POST){
		
		//monta os arrays das perguntas 12 e 18
		$arritem12 = array();
		$sql = "SELECT iulid FROM pse.iulitempergunta
				WHERE pulid = 12 and iuldescricao<>'Outros' order by iulid";
		$dados = $db->carregar($sql);
		$z = 0;
		foreach($dados as $dados){
			$arritem12[$z] = $dados['iulid'];
			$z++;
		}
		$arritem17 = array();
		$sql = "SELECT iulid FROM pse.iulitempergunta
				WHERE pulid = 18 order by iulid";
		$dados = $db->carregar($sql);
		$z = 0;
		foreach($dados as $dados){
			$arritem17[$z] = $dados['iulid'];
			$z++;
		}
		//******************************************************
		
		
		//******** Grava a pergunta 2.3*****************************************
		$sql = "SELECT rulid FROM pse.rulresposta
				WHERE espid = $espid AND pulid = 17";
		$rulid17 = $db->pegaUm($sql);
		$sql = "SELECT rulid FROM pse.rulresposta
					WHERE pulid = 12 and espid = $espid";
		$rulid = $db->pegaUm($sql);
		$sql = "SELECT rulid FROM pse.rulresposta
					WHERE pulid = 18 and espid = $espid";
		$rulid18 = $db->pegaUm($sql);
//		ver ($rulid, $rulid17, $rulid18);

		if(empty($_REQUEST[rulflagresposta_17])){
			echo "<script>alert('A pergunta 2.3 é obrigatória!');</script>";
			exit;
		} else {
			if($rulid17==''){
				$sql = "INSERT INTO pse.rulresposta
						(pulid,espid,paqid,rulflagresposta)
						VALUES
						(17,$espid,$ano,'$_REQUEST[rulflagresposta_17]')
						RETURNING rulid";
				$rulid17 = $db->pegaUm($sql);
			}
			else {
				if($_REQUEST['rulflagresposta_17']=='n'){
					if($rulid18){
						$sql = "SELECT count(*) FROM pse.isuitemselecionado i
								INNER JOIN
									pse.asuselecionada a ON a.isuid=i.isuid
								WHERE i.iulid in (".implode(",",$arritem17).") AND i.rulid = $rulid18";
						$qd = $db->pegaUm($sql);
						if($qd>0){
							echo "<script>alert('Existe(m) Subação(ões) vinculadas!');</script>";
						}
						else {
							$sql = "DELETE FROM pse.isuitemselecionado
									WHERE rulid = $rulid";
							$db->executar($sql);
							$sql = "DELETE FROM pse.rulresposta
									WHERE rulid = $rulid";
							$db->executar($sql);
							$sql = "UPDATE pse.rulresposta SET
								rulflagresposta = '$_REQUEST[rulflagresposta_17]'
								WHERE
								pulid = 17 AND
								espid = $espid AND
								paqid = $ano";
							$db->executar($sql);
							echo "<script>alert('Operação realizada com sucesso!');</script>";
						}
					} else {
							if ($rulid){
							$sql = "DELETE FROM pse.isuitemselecionado
									WHERE rulid = $rulid";
							$db->executar($sql);
							$sql = "DELETE FROM pse.rulresposta
									WHERE rulid = $rulid";
							$db->executar($sql);
							$sql = "UPDATE pse.rulresposta SET
								rulflagresposta = '$_REQUEST[rulflagresposta_17]'
								WHERE
								pulid = 17 AND
								espid = $espid AND
								paqid = $ano";
							$db->executar($sql);
							echo "<script>alert('Operação realizada com sucesso!');</script>";
							}
							else{
								echo "<script>alert('Operação já executada!');</script>";
							}
					}
				} else {
					$sql = "UPDATE pse.rulresposta SET
							rulflagresposta = '$_REQUEST[rulflagresposta_17]'
							WHERE
							pulid = 17 AND
							espid = $espid AND
							paqid = $ano";
					$db->executar($sql);
					echo "<script>alert('Operação realizada com sucesso!');</script>";
				}
			}
		//******** Fim Grava a pergunta 2.3****************************************

			
		//************** pergunta 2.4  *********************************
			
			if($rulid==''){
				$sql = "INSERT INTO pse.rulresposta
						(pulid,espid,paqid,rulflagresposta)
						VALUES
						(12,$espid,$ano,'s')
						RETURNING rulid";
				$rulid = $db->pegaUm($sql);
			}

			$valores = $_REQUEST['perg_12']; 	
			if($_REQUEST['rulflagresposta_17']=='s'){
				if(empty($valores)){
					echo "<script>alert('Como a resposta da pergunta 2.3 foi \"SIM\", a pergunta 2.4 é obrigatória!');</script>";
				}
				else {
					if($rulid18){
						//Verifica se tem subacao
						$sql = "SELECT i.iulid FROM pse.isuitemselecionado i
								INNER JOIN
									pse.asuselecionada a ON a.isuid=i.isuid
								WHERE i.rulid = $rulid18";
						$dados = $db->carregar($sql);
						$z = 0;
						//troca o ID da 18 para a 12
						if($dados){
							foreach($dados as $dados){
								$itens18[$z] = $dados['iulid'];
								$key = array_search($dados['iulid'], $arritem17);
								$item12[$z] = $arritem12[$key];
								$z++;
							}
						}
					}
					//Deleta todos os registros onde não tem subacao vinculada
					$sql = "DELETE FROM pse.isuitemselecionado
							WHERE rulid = $rulid";
					if($item12){
						$sql.=" and iulid not in (".implode(',',$item12).")";
					}

					$db->executar($sql);
					
					//Insere os registro selecionados nos checks
					foreach ($valores as $item) {
						if(!empty($item12)){
							if (in_array($item, $item12)) {
							    $id = 1;
							}
						}
						
						if(!$id){
							$sql = "INSERT INTO pse.isuitemselecionado
									(iulid,rulid)
									VALUES
									($item,$rulid)";
							$isuid = $db->executar($sql);
						}
					}
				}
			}
		}
	}
			

//Verifica se existe resposta já grava no BD	
$sql = "SELECT rulflagresposta from pse.rulresposta
		WHERE pulid = 17 and espid = $espid";
$radios = $db->pegaUm($sql);	
$radios = ($radios==''?'':$radios);
if($radios!="")
	$radios != "s" ? $checkNao = "checked" : $checkSim = "checked";
?>

<script language="JavaScript" src="../includes/funcoes.js"></script>

<form method="POST"  name="formulario">
<?php Cabecalho($entid,1); ?> 
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><input type='hidden' name='nrperguntas' id='nrperguntas' value='<?=$perguntaPage;?>'></td>
	</tr>
	<?php 
		$idpergunta = explode(",",$perguntaPage);
		foreach($idpergunta as $pergunta){
			perguntasULI($pergunta,$espid); 
		}
	?>
	<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
		<tr style="background-color: #DCDCDC">
			<td colspan="2" align="center"><input type="submit" value="Salvar"></td>
		</tr>
	<?php }
		$sql = "SELECT rulid from pse.rulresposta
		WHERE pulid = 12 and espid = $espid";
		$rulidT = $db->pegaUm($sql);
		$proxima = 0;
		if( $rulidT ){
			$sql = "SELECT iulid from pse.isuitemselecionado
			WHERE rulid = $rulidT";
			$arrR = $db->carregar($sql);
			if( is_array( $arrR ) ){
				foreach( $arrR as $rsp ){
					if( $rsp['iulid'] <> 323 ){
						$proxima = 1;
					}
				}
			}
		}
		
		$pagina = $proxima == 1 ? 'compInecessidadeCarencia03' : 'compInecessidadeCarencia04';
		echo navegacao('compInecessidadeCarencia01',$pagina);
	?>
	</table>
</form>	

<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
function  check(opcao){
	if(opcao=='s'){
		$('itens12').show();
	}
	else
		$('itens12').hide();
}
check('<?= $radios; ?>');
</script>