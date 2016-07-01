<?php
include_once( APPRAIZ. "pse/classes/RulResposta.class.inc" );
include_once( APPRAIZ. "pse/classes/IsuItemSelecionado.class.inc" );

	
class PulPergunta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.pulpergunta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pulid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pulid' => null, 
									  	'pulboleana' => null, 
									  	'pulflagitem' => null, 
									  	'pulflagmultiescolha' => null, 
									  	'puldescricao' => null, 
									  	'pulordem' => null, 
									  );
	
	public $metodoPergunta = null;								  
									  
	function carregarPerguntas($pulordem, $rulid = null, $espid = null )
	{
		if($rulid){
			$rulid = $rulid;
		}else{
			$rulid = '';
		}
		
		if($espid){
			$espid = $espid;
		}else{
			$espid = '';
		}

		$sql = "SELECT * FROM pse.pulpergunta WHERE pulordem in ({$pulordem}) ORDER BY pulordem";
		$rs = $this->carregar($sql);
		
		if($rs)
		{
			foreach ($rs as $values)
			{
				echo "<tr>";
				echo "<td class='SubTituloEsquerda' colspan='2'>{$values["puldescricao"]}</td>";
				echo "</tr>";
				
				if($values["pulboleana"] == 'n' && $values["pulflagitem"] == 's'  && $values["pulflagmultiescolha"]== 's' && $values["pulordem"]== '1.   ')
				{
					echo "<tr>";
					echo"<td colspan='2'>";
					
					$obResposta = new RulResposta($rulid,1, $espid);
			
					$this->pulid 	= $values["pulid"];
					$matriz 		= $this->carregarOutrosAtores();
					$carregados 	= $this->carregaOutrosAtoresCarregados ($obResposta->rulid);
								
					$campoAgrupador = new Agrupador( 'formunicipio' );
					$campoAgrupador->setOrigem( 'agrupadorAtores', null, $matriz );
					$campoAgrupador->setDestino( 'atores',null, $carregados );
					$campoAgrupador->exibir();
							
					echo "</td>";
					echo "</tr>";
				}
				if($values["pulboleana"] == 'n' && $values["pulflagitem"] == 's'  && $values["pulflagmultiescolha"]== 's' && $values["pulordem"]== '2.   ')
				{
					echo "<tr>";
					echo "<td style='text-align: left'>";
						
					$obResposta = new RulResposta($rulid,2, $espid);
					$checkNao = "";
					$checkSim = "";
							
					if ($obResposta->rulflagresposta == "n" ){
						$checkNao = "checked=checked";
					}
					if ($obResposta->rulflagresposta == "s" ){
						$checkSim = "checked=checked";
					}
					$check = new cls_banco();
					$sql = "SELECT iuldescricao FROM pse.iulitempergunta
							WHERE pulid=2 AND iulflagtexto='n' AND iulflagvalor='n' AND iulflagsubitem='n'";
					$c = $check->pegaUm($sql);
					echo "<input type='checkbox' id='checkparcerias' $checkNao name='checkparcerias' onclick='checkParcerias()'>".$c;
					echo "</td>";
					echo "</tr>";
					$sql = "SELECT iulid as codigo, iuldescricao as descricao FROM pse.iulitempergunta
							WHERE pulid=2 AND iulflagtexto='s' AND iulflagvalor='n' AND iulflagsubitem='n'";
					echo "<tr>";
					echo "<td style='text-align: left'>";
					echo "{$check->monta_combo( 'parcerias', $sql, 'S', 'Selecione uma Parceria', 'carregaParcerias', '', '', '250','S','parcerias','','','Parcerias Locais' )}";
					echo "</td>";
					echo "<td>";
					echo 'Qual: <input type="text" id="texto" name="texto" value="" title="Texto" maxlength="100" size="120"><img border="0" src="../imagens/obrig.gif"/>';
					echo "</td>";
					echo "</tr>";
				}
				if($values["pulboleana"] == 'n' && $values["pulflagitem"] == 's'  && $values["pulflagmultiescolha"]== 's' && $values["pulordem"]== '3.   ')
				{
					echo "<tr>";
					echo "<td style='text-align: left'>";
					
					$obResposta = new RulResposta($rulid,3, $espid);
					$checkNao = "";
					$checkSim = "";
							
					if ($obResposta->rulflagresposta == "n" ){
						$checkNao = "checked=checked";
					}
					if ($obResposta->rulflagresposta == "s" ){
						$checkSim = "checked=checked";
					}
					$check = new cls_banco();
								
					$sql ="SELECT iulid as codigo, iuldescricao as descricao, iulflagtexto as flag FROM pse.iulitempergunta
							WHERE pulid=3 AND iulflagvalor='n' AND iulflagsubitem='n'";;
					$c = $check->carregar($sql);
					foreach($c as $linha){
						$check1 = "";
						$texto = "";
						if ($rulid<>'') {
							$sql ="SELECT iulid FROM pse.isuitemselecionado WHERE rulid = {$rulid} AND iulid = {$linha['codigo']} ";
							$iulid = $check->pegaUm($sql);
							if ($iulid == $linha['codigo']) { $check1 = "checked=checked";}
						}
						echo "<tr>";
						echo "<td colspan='2' style='text-align: left'>";
						if($linha['flag']=='n'){
							echo '<input type="checkbox" value="'.$linha['codigo'].'" '.$check1.' name="check1[]" id="check1[]"/>'.$linha['descricao'];
						} else if($linha['flag']=='s'){
							if($rulid<>null){
								$sql ="SELECT isuid FROM pse.isuitemselecionado WHERE rulid = {$rulid} AND iulid = 32";
								$isuid = $check->pegaUm($sql);
								if($isuid<>''){
									$sql ="SELECT tultexto FROM pse.tultextoitemselecionado WHERE isuid = {$isuid}";
									$texto = $check->pegaUm($sql);
								}
							}
							echo '<input type="checkbox" value="'.$linha['codigo'].'" '.$check1.' name="check1[]" id="check1[]"/>'.$linha['descricao'].' <input type="text" id="texto" name="texto" value="'.$texto.'" title="Texto" maxlength="80" size="100">';
						}
						echo "</td>";
						echo "</tr>";
					}							
					echo "</td>";
					echo "</tr>";
				}
				if($values["pulboleana"] == 'n' && $values["pulflagitem"] == 's'  && $values["pulflagmultiescolha"]== 'n' && $values["pulordem"]== '4.   ')
				{
					echo "<tr>";
					echo "<td style='text-align: left'>";
							
					$obResposta = new RulResposta($rulid,4, $espid);
					$checkNao = "";
					$checkSim = "";
						
					if ($obResposta->rulflagresposta == "n" ){
						$checkNao = "checked=checked";
					}
					if ($obResposta->rulflagresposta == "s" ){
						$checkSim = "checked=checked";
					}
					$check = new cls_banco();
						
					$sql ="SELECT iulid as codigo, iuldescricao as descricao FROM pse.iulitempergunta
							WHERE pulid=4";
					$c = $check->carregar($sql);
					foreach($c as $linha){
						$check1 = "";
						if ($rulid<>'') {
							$sql ="SELECT iulid FROM pse.isuitemselecionado WHERE rulid = {$rulid} AND iulid = {$linha['codigo']} ";
							$iulid = $check->pegaUm($sql);
							if ($iulid == $linha['codigo']) { $check1 = "checked=checked";}
						}
						echo "<tr>";
						echo "<td colspan='2' style='text-align: left'>";
						echo '<input type="radio" title="Questão 4" value="'.$linha['codigo'].'" '.$check1.' name="radio1[]" id="radio1[]"/>'.$linha['descricao'];
						echo "</td>";
						echo "</tr>";
					}							
					echo "</td>";
					echo "</tr>";
				}
				if($values["pulboleana"] == 'n' && $values["pulflagitem"] == 's'  && $values["pulflagmultiescolha"]== 'n' && $values["pulordem"]== '5.   ')
				{
					echo "<tr>";
					echo "<td style='text-align: left'>";
							
					$obResposta = new RulResposta($rulid,5, $espid);
					$checkNao = "";
					$checkSim = "";
							
					if ($obResposta->rulflagresposta == "n" ){
						$checkNao = "checked=checked";
					}
					if ($obResposta->rulflagresposta == "s" ){
						$checkSim = "checked=checked";
					}
					$check = new cls_banco();
							
					$sql ="SELECT iulid as codigo, iuldescricao as descricao FROM pse.iulitempergunta
							WHERE pulid=5";;
					$c = $check->carregar($sql);
					foreach($c as $linha){
						$check1 = "";
						if ($rulid<>'') {
							$sql ="SELECT iulid FROM pse.isuitemselecionado WHERE rulid = {$rulid} AND iulid = {$linha['codigo']} ";
							$iulid = $check->pegaUm($sql);
							if ($iulid == $linha['codigo']) { $check1 = "checked=checked";}
						}
						echo "<tr>";
						echo "<td colspan='2' style='text-align: left'>";
						echo '<input type="radio" title="Questão 5" value="'.$linha['codigo'].'" '.$check1.' name="radio2[]" id="radio2[]"/>'.$linha['descricao'];
						echo "</td>";
						echo "</tr>";
					}							
					echo "</td>";
					echo "</tr>";
				}
				if($values["pulboleana"] == 's' && $values["pulflagitem"] == 'n'  && $values["pulflagmultiescolha"]== 'n' && $values["pulordem"]== '6.   ')
				{
					echo "<input  type='hidden' name='idpergunta' value ={$values["pulid"]}>";
					echo "<tr>";
					echo "<td>";
						
					$obResposta = new RulResposta($rulid,6, $espid);
					$checkNao = "";
					$checkSim = "";
					if($rulid<>''){
						$obResposta->rulflagresposta != "s" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
					}	
					echo "<input type='radio' value='n' $checkNao name='per_{$values["pulid"]}' title='Questão 6' onclick=\"". str_replace("validaItem(0,","validaItem(0, ", $this->metodoPergunta). "\" />Não"; 
					echo "<input type='radio' value='s' $checkSim name='per_{$values["pulid"]}' title='Questão 6' onclick=\"". str_replace("validaItem(0,","validaItem(1, ", $this->metodoPergunta). "\" />Sim"; 
					echo "</td>";
					echo "</tr>";
					echo"<tr name='item6[]' style='display: none;'>";
					echo "<td class='SubTituloEsquerda' colspan='2'>6.1. Quais as informações relativas à situação de saúde do município que a Unidade Local utilizou para a construção da análise de situação em saúde?</td>";
					echo "</tr>";
					echo "<tr name='item6[]'  style='display: none;'>";
					echo"<td colspan='2' align='left'>";
		
					$obResposta = new RulResposta($rulid,7, $espid);
			
					$this->pulid 	= 7;
					$matriz 		= $this->carregarOutrosAtores();
					$carregados 	= $this->carregaOutrosAtoresCarregados ($obResposta->rulid);
								
					$campoAgrupador = new Agrupador( 'formunicipio' );
					$campoAgrupador->setOrigem( 'agrupadorAtores', null, $matriz );
					$campoAgrupador->setDestino( 'atores',null, $carregados );
					$campoAgrupador->exibir();
							
					echo "</td>";
					echo "</tr>";
				}
				if($values["pulboleana"] == 's' && $values["pulflagitem"] == 'n'  && $values["pulflagmultiescolha"]== 'n' && $values["pulordem"]== '7.   ')
				{
					echo "<input  type='hidden' name='idpergunta' value ={$values["pulid"]}>";
					echo "<tr>";
					echo "<td>";
						
					$obResposta = new RulResposta($rulid,8, $espid);
					$checkNao = "";
					$checkSim = "";
					if($rulid<>''){
						$obResposta->rulflagresposta != "s" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
					}	
					echo "<input type='radio' value='n' $checkNao name='per_{$values["pulid"]}' title='Questão 7' onclick=\"". str_replace("validaItem(0,","validaItem(0, ", $this->metodoPergunta). "\" />Não";
					echo "<input type='radio' value='s' $checkSim name='per_{$values["pulid"]}' title='Questão 7' onclick=\"". str_replace("validaItem(0,","validaItem(1, ", $this->metodoPergunta). "\" />Sim"; 
					echo "</td>";
					echo "</tr>";
					
					echo"<tr name='item7[]' style='display: none;'>";
					echo "<td class='SubTituloEsquerda' colspan='2'>7.1. Quais as informações do CENSO ESCOLAR que a Unidade Local Intersetorial do PSE utilizou para a construção do projeto municipal?</td>";
					echo "</tr>";
					
					echo "<tr name='item7[]'  style='display: none;'>";
					echo"<td colspan='2'>";
					$check = new cls_banco();
							
					$sql ="SELECT
								iulid as codigo, 
								iuldescricao || ' <br> ' as descricao
							FROM 
								pse.iulitempergunta
							WHERE
								pulid=9";
					$c = $check->carregar($sql);
					foreach($c as $linha){
						$check1 = "";
						if ($rulid<>'') {
							$sql ="SELECT iulid FROM pse.isuitemselecionado WHERE rulid = {$rulid} AND iulid = {$linha['codigo']} ";
							$iulid = $check->pegaUm($sql);
							if ($iulid == $linha['codigo']) { $check1 = "checked=checked";}
						}
						echo '<input type="checkbox" value="'.$linha['codigo'].'" '.$check1.' name="check2[]" id="check2[]"/>'.$linha['descricao'];
					}
					echo "</td>";
					echo "</tr>";
				}
				if($values["pulboleana"] == 's' && $values["pulflagitem"] == 's'  && $values["pulflagmultiescolha"]== 'n' && $values["pulordem"]== '5.4. ')
				{
					echo "<input  type='hidden' name='idpergunta' value ={$values["pulid"]}>";
					echo "<tr>";
					echo "<td>";
						
					$obResposta = new RulResposta($rulid,28, $espid);
					$checkNao = "";
					$checkSim = "";
					$tultexto = "";
					if($rulid<>''){
						$obResposta->rulflagresposta != "s" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
					}	
					echo "<input type='radio' value='n' $checkNao name='per_{$values["pulid"]}' title='Questão 5.4' onclick=\"". str_replace("validaItem(0,","validaItem(0, ", $this->metodoPergunta). "\" />Não"; 
					echo "<input type='radio' value='s' $checkSim name='per_{$values["pulid"]}' title='Questão 5.4' onclick=\"". str_replace("validaItem(0,","validaItem(1, ", $this->metodoPergunta). "\" />Sim"; 
					echo "</td>";
					echo "</tr>";
					
					echo"<tr name='item28[]' style='display: none;'>";
					echo "<td class='SubTituloEsquerda' colspan='2'>5.5. De que maneira?</td>";
					echo "</tr>";
					
					echo "<tr name='item28[]'  style='display: none;'>";
					echo"<td colspan='2' align='left'>";
					if($rulid<>''){
						$check = new cls_banco();
						$sql = "SELECT iulid from pse.iulitempergunta where pulid = 28";
						$iulid = $check->pegaUm($sql);
						$sql ="SELECT isuid FROM pse.isuitemselecionado WHERE rulid = {$rulid} AND iulid = $iulid";
						$isuid = $check->pegaUm($sql);
						if($isuid<>''){
							$sql ="SELECT tultexto FROM pse.tultextoitemselecionado WHERE isuid = {$isuid}";
							$tultexto = $check->pegaUm($sql);
						}
					}
					echo campo_textarea( 'tultexto', 'N', 'S', 'Texto', '100', '10', '1000', '', '', '', false, '', $tultexto );
					echo "</td>";
					echo "</tr>";
				}
			}
		}	
	}
	
	function carregarOutrosAtores()
	{
		//CARREGANDO OUTROS ATORES
		$sql= "SELECT 
				iulid as codigo,
				iuldescricao as descricao 
			  FROM 
				pse.iulitempergunta
			  WHERE
			  	pulid in ({$this->pulid})";
		
		$rs = $this->carregar($sql);
		
		$matriz = array();
		if($rs)
		{
			foreach($rs as $value)
			{
				$matriz[] = array("codigo"=> $value["codigo"],"descricao"=> $value["descricao"]);
			}
			
		}
		
		if(count($matriz) == 0)
					$matriz = null;
					
		return $matriz;
	}
	
	function carregaOutrosAtoresCarregados($rulid)
	{
		$matriz = array();
		
		if($rulid)
		{
			$sql = "SELECT
						itp.iulid as codigo, itp.iuldescricao as descricao
					FROM
						pse.isuitemselecionado its
					INNER JOIN pse.rulresposta AS res ON res.rulid = its.rulid 
					INNER JOIN pse.iulitempergunta AS itp ON itp.iulid = its.iulid		
					WHERE
						itp.pulid = {$this->pulid} AND
						res.rulid = {$rulid} ";
			$rs = $this->carregar($sql);
			
			
			if($rs)
			{
				foreach($rs as $value)
				{
					$matriz[] = array("codigo"=> $value["codigo"],"descricao"=> $value["descricao"]);
				}
				
			}
		}
		
		if(count($matriz) == 0)
					$matriz = null;
					
		return $matriz;			
	}
}