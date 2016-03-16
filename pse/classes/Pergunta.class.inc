<?php
include_once( APPRAIZ. "pse/classes/Resposta.class.inc" );
include_once( APPRAIZ. "pse/classes/ItemSelecionado.class.inc" );
	
class Pergunta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.pergunta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "perid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'perid' => null, 
									  	'perflagboleana' => null, 
									  	'perflagitem' => null, 
									  	'perflagmultiescolha' => null, 
									  	'perdescricao' => null, 
									  	'pernumeroordem' => null, 
									  );
	
	public $metodoPergunta = null;								  
									  
	function carregarPerguntas($pernumeroordem, $resid = null, $empid = null )
	{
		if($resid){
			$resid = $resid;
		}else{
			$resid = '';
		}
		
		if($empid){
			$empid = $empid;
		}else{
			$empid = '';
		}

		$sql = "SELECT * FROM pse.pergunta WHERE pernumeroordem in ({$pernumeroordem}) ORDER BY pernumeroordem";
		$rs = $this->carregar($sql);
		
		if($rs)
		{
			foreach ($rs as $values)
			{
				if ($values["pernumeroordem"]=='3.8  ' || $values["pernumeroordem"]=='3.9  ' )
				{
					echo "<tr>";
					echo "<td class='SubTituloEsquerda' colspan='2'> {$values["perdescricao"]}</td>";
					echo "</tr>";
				}
				if ($values["pernumeroordem"]!='3.8  ' && $values["pernumeroordem"]!='3.9  ' )
				{
					echo "<tr>";
					echo "<td class='SubTituloEsquerda' colspan='2'>{$values["pernumeroordem"]} {$values["perdescricao"]}</td>";
					echo "</tr>";
				}
				if($values["perflagboleana"] == 's' && $values["perflagitem"] == 'n'  && $values["perflagmultiescolha"]== 'n' && $values["pernumeroordem"]== '3.8  ')
				{
					if($values["perid"] == '14')
					{				
					echo "<tr>";
						echo "<td>";
							$obResposta = new Resposta('',$values["perid"], $empid);
							
							$checkNao = "";
							$checkSim = "";
							if ($obResposta->resflagresposta == "n" ){
								$checkNao = "checked=checked";
							}
							if ($obResposta->resflagresposta == "s" ){
								$checkSim = "checked=checked";
							}
							
							/*echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick='pse20102011(this.value)' />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick='pse20102011(this.value)' />Sim";*/
							echo "<input type='radio' value='n' $checkNao name='p2er_{$values["perid"]}' />Não existe pólo da rede UAB no município"; 
							echo "<input type='radio' value='s' $checkSim name='p2er_{$values["perid"]}' />Existe pólo da rede UAB no município";
						echo "</td>";
					echo "</tr>";
					} else {
					echo "<tr>";
						echo "<td>";
							$obResposta = new Resposta('',$values["perid"], $empid);
							
							$checkNao = "";
							$checkSim = "";
							if ($obResposta->resflagresposta == "n" ){
								$checkNao = "checked=checked";
							}
							if ($obResposta->resflagresposta == "s" ){
								$checkSim = "checked=checked";
							}
							
							/*echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick='pse20102011(this.value)' />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick='pse20102011(this.value)' />Sim";*/
							echo "<input type='radio' value='n' $checkNao name='p2er_{$values["perid"]}' />Não existe núcleo de tele-saúde no município"; 
							echo "<input type='radio' value='s' $checkSim name='p2er_{$values["perid"]}' />Existe núcleo de tele-saúde no município";
						echo "</td>";
					echo "</tr>";
					}
				}
							if($values["perflagboleana"] == 's' && $values["perflagitem"] == 'n'  && $values["perflagmultiescolha"]== 'n' && $values["pernumeroordem"]!= '3.8  ' && $values["pernumeroordem"]!= '2.1.4' && $values["pernumeroordem"]!= '2.1.3')
				{
					echo "<tr>";
						echo "<td>";

							$obResposta = new Resposta($resid,$values["perid"], $empid);
							
							$checkNao = "";
							$checkSim = "";
							if ($obResposta->resflagresposta == "n" ){
								$checkNao = "checked=checked";
							}
							if ($obResposta->resflagresposta == "s" ){
								$checkSim = "checked=checked";
							}
							
							/*echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick='pse20102011(this.value)' />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick='pse20102011(this.value)' />Sim";*/
							echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' />Sim";
						echo "</td>";
					echo "</tr>";
				}
				if($values["perflagboleana"] == 's' && $values["perflagitem"] == 'n'  && $values["perflagmultiescolha"]== 'n' && $values["pernumeroordem"]== '2.1.3')
				{
					echo "<tr>";
						echo "<td>";

							$obResposta = new Resposta($resid,$values["perid"], $empid);
							
							$checkNao = "";
							$checkSim = "";
							if($resid<>''){
								$obResposta->resflagresposta != "s" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
							}
							
							/*echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick='pse20102011(this.value)' />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick='pse20102011(this.value)' />Sim";*/
							echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' />Sim";
						echo "</td>";
					echo "</tr>";
				}
				if($values["perflagboleana"] == 's' && $values["perflagitem"] == 's'  && $values["perflagmultiescolha"]== 's' && $values["pernumeroordem"]== '1.3  ') //3.2, 3.3 e 3.4 por exemplo
				{
					echo "<input  type='hidden' name='idpergunta' value ={$values["perid"]}>";
					echo "<tr>";
						echo "<td>";
							$obResposta = new Resposta($resid,1, $empid);
							$checkNao = "";
							$checkSim = "";
						if($resid<>''){
							$obResposta->resflagresposta != "s" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
						}
							echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick=\"". str_replace("validaItem(0,","validaItem(0, ", $this->metodoPergunta). "\" />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick=\"". str_replace("validaItem(0,","validaItem(1, ",$this->metodoPergunta). "\" />Sim";
						echo "</td>";
					echo "</tr>";
					
					echo"<tr name='item1_3[]' style='display: none;'>";
						echo "<td class='SubTituloEsquerda' colspan='2'>Selecione representantes de outras áreas que integram o Grupo de Trabalho Intersetorial do PSE:</td>";
					echo "</tr>";
					
					echo "<tr name='item1_3[]'  style='display: none;'>";
					echo"<td colspan='2' align='center'>";
					
								$this->perid 	= $values["perid"];
								$matriz 		= $this->carregarOutrasSecretarias();
								$carregados 	= $this->carregaOutrasSecretariasCarregadas ($obResposta->resid); //  $obResposta->resid;
								
								$campoAgrupador = new Agrupador( 'formunicipio' );
								$campoAgrupador->setOrigem( 'agrupadorSecretarias', null, $matriz );
								$campoAgrupador->setDestino( 'secretarias',null, $carregados );
								$campoAgrupador->exibir();
							
					echo "</td>";
					echo "</tr>";
				}
				if($values["perflagboleana"] == 's' && $values["perflagitem"] == 's'  && $values["perflagmultiescolha"]== 's' && $values["pernumeroordem"]== '2.1.1') //3.2, 3.3 e 3.4 por exemplo
				{
					echo "<input  type='hidden' name='idpergunta' value ={$values["perid"]}>";
					echo "<tr>";
						echo "<td>";
						
							$obResposta = new Resposta($resid,6, $empid);
							$checkNao = "";
							$checkSim = "";
							if($resid<>''){
								$obResposta->resflagresposta != "s" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
							}	
							echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick=\"". str_replace("validaItem(0,","validaItem(0, ", $this->metodoPergunta). "\" />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick=\"". str_replace("validaItem(0,","validaItem(1, ", $this->metodoPergunta). "\" />Sim"; 
						echo "</td>";
					echo "</tr>";
					
					echo"<tr name='item2_11[]' style='display: none;'>";
						echo "<td class='SubTituloEsquerda' colspan='2'>Quais as informações relativas a situação de saúde do município que o GTI utilizou para a construção da análise de situação em saúde?</td>";
					echo "</tr>";
					
					echo "<tr name='item2_11[]'  style='display: none;'>";
					echo"<td colspan='2' align='center'>";
					
								$this->perid 	= $values["perid"];
								$matriz 		= $this->carregarOutrasSecretarias();
								$carregados 	= $this->carregaOutrasSecretariasCarregadas ($obResposta->resid); //  $obResposta->resid;
								
								$campoAgrupador = new Agrupador( 'formunicipio' );
								$campoAgrupador->setOrigem( 'agrupadorSitSaude', null, $matriz );
								$campoAgrupador->setDestino( 'SitSaude',null, $carregados );
								$campoAgrupador->exibir();
							
					echo "</td>";
					echo "</tr>";
				}
			if($values["perflagboleana"] == 's' && $values["perflagitem"] == 's'  && $values["perflagmultiescolha"]== 's' && $values["pernumeroordem"]== '2.1.2') //3.2, 3.3 e 3.4 por exemplo
				{
							//dbg($pernumeroordem);
							//dbg($resid);
							//dbg($empid);
					echo "<input  type='hidden' name='idpergunta' value ={$values["perid"]}>";
					echo "<tr>";
						echo "<td>";
						
							$obResposta = new Resposta($resid,7, $empid);
							$checkNao = "";
							$checkSim = "";
							if($resid<>''){
								$obResposta->resflagresposta != "s" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
							}	
							echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick=\"". str_replace("validaItem(0,","validaItem(0, ", $this->metodoPergunta). "\" />Não";
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick=\"". str_replace("validaItem(0,","validaItem(1, ", $this->metodoPergunta). "\" />Sim"; 
						echo "</td>";
					echo "</tr>";
					
					echo"<tr name='item2_12[]' style='display: none;'>";
						echo "<td class='SubTituloEsquerda' colspan='2'>Quais as informações do CENSO ESCOLAR que o GTI utilizou para a construção do projeto municipal?</td>";
					echo "</tr>";
					
					echo "<tr name='item2_12[]'  style='display: none;'>";
					echo"<td colspan='2'>";
								$check = new cls_banco();
								
								$sql ="SELECT
												iteid as codigo, 
												itedescricao || ' <br> ' as descricao
										FROM 
												pse.itempergunta
										WHERE
												perid=7";
								$c = $check->carregar($sql);
								foreach($c as $linha){
									$check1 = "";
									if ($resid<>'') {
										$sql ="SELECT iteid FROM pse.itemselecionado WHERE resid = {$resid} AND iteid = {$linha['codigo']} ";
										$iteid = $check->pegaUm($sql);
										if ($iteid == $linha['codigo']) { $check1 = "checked=checked";}
									}
									echo '<input type="checkbox" value="'.$linha['codigo'].'" '.$check1.' name="censo2[]" id="censo2[]"/>'.$linha['descricao'];
								}
								//$check->monta_checkbox("censo2[]", $sql, $objItemSelecionado->censo2, '');
							
					echo "</td>";
					echo "</tr>";
				}
				if($values["perflagboleana"] == 's' && $values["perflagitem"] == 'n'  && $values["perflagmultiescolha"]== 'n' && $values["pernumeroordem"]== '2.1.4') //3.2, 3.3 e 3.4 por exemplo
				{
							//dbg($pernumeroordem);
							//dbg($resid);
							//dbg($empid);
					echo "<input  type='hidden' name='idpergunta' value ={$values["perid"]}>";
					echo "<tr>";
						echo "<td>";
						
							$obResposta = new Resposta($resid,9, $empid);
							$checkNao = "";
							$checkSim = "";
							if($resid<>''){
								$obResposta->resflagresposta != "s" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
							}	
							echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick=\"". str_replace("validaItem(0,","validaItem(0, ", $this->metodoPergunta). "\" />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick=\"". str_replace("validaItem(0,","validaItem(1, ", $this->metodoPergunta). "\" />Sim"; 
							//echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick='validaItem(0,\"item2_14[]\")' />Não"; 
							//echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick='validaItem(1,\"item2_14[]\")' />Sim";
						echo "</td>";
					echo "</tr>";
					
					echo"<tr name='item2_14[]' style='display: none;'>";
						echo "<td class='SubTituloEsquerda' colspan='2'>Quais os parceiros intersetoriais do PSE Municipal?</td>";
					echo "</tr>";
					
					echo "<tr name='item2_14[]'  style='display: none;'>";
					echo"<td colspan='2'>";
					
					$check = new cls_banco();
								
								$sql ="SELECT
												iteid as codigo, 
												itedescricao || ' <br> ' as descricao
										FROM 
												pse.itempergunta
										WHERE
												perid=9";
								$c = $check->carregar($sql);
								foreach($c as $linha){
									$check1 = "";
									if ($resid<>'') {
										$sql ="SELECT iteid FROM pse.itemselecionado WHERE resid = {$resid} AND iteid = {$linha['codigo']} ";
										$iteid = $check->pegaUm($sql);
										if ($iteid == $linha['codigo']) { $check1 = "checked=checked";}
									}
									echo '<input type="checkbox" value="'.$linha['codigo'].'" '.$check1.' name="censo3[]" id="censo3[]"/>'.$linha['descricao'];
								}
								//$check->monta_checkbox("censo3", $sql, $objPseAnoEscolaMun->censo2, '');
							
					echo "</td>";
					echo "</tr>";
				}
				if($values["perflagboleana"] == 'n' && $values["perflagitem"] == 's'  && $values["perflagmultiescolha"]== 'n' && $values["pernumeroordem"]== '3.1. ')
				{
					echo "<input  type='hidden' name='idpergunta' value ={$values["perid"]}>";
					echo "<tr>";
						echo "<td>";

							//$obResposta = new Resposta($resid,$values["perid"], $empid);
							
							/*echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick='pse20102011(this.value)' />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick='pse20102011(this.value)' />Sim";*/
							$check = new cls_banco();
							$sql ="SELECT
											iteid as codigo, 
											itedescricao as descricao
									FROM 
											pse.itempergunta
									WHERE
											perid=10";
								$c = $check->carregar($sql);
								foreach($c as $linha){
									if($resid){
										$resid = $resid;
									}else{
										$resid = '0';
									}
									$check1 = "";
									$sql ="SELECT iteid FROM pse.itemselecionado WHERE resid = {$resid} AND iteid = {$linha['codigo']} ";
									$iteid = $check->pegaUm($sql);
									if ($iteid == $linha['codigo']) { $check1 = "checked=checked";}
									echo '<input type="radio" value="'.$linha['codigo'].'" '.$check1.' name="per_10" id="per_10"/>'.$linha['descricao'].'<br>';
								}
						echo "</td>";
					echo "</tr>";
				}
			if($values["perflagboleana"] == 's' && $values["perflagitem"] == 's'  && $values["perflagmultiescolha"]== 's' && $values["pernumeroordem"]== '3.2  ')
				{
					echo "<input  type='hidden' name='idpergunta' value ={$values["perid"]}>";
					echo "<tr>";
						echo "<td>";
						
							$obResposta = new Resposta($resid,11, $empid);
							$checkNao = "";
							$checkSim = "";
							if($resid<>''){
								$obResposta->resflagresposta != "s" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
							}	
							
							echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick='validaItem(0,\"item3_2[]\")' />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick='validaItem(1,\"item3_2[]\")' />Sim";
						echo "</td>";
					echo "</tr>";
					
					echo"<tr name='item3_2[]' style='display: none;'>";
						echo "<td class='SubTituloEsquerda' colspan='2'>3.3. Que tipos de dificuldades costuma ter?</td>";
					echo "</tr>";
					
					echo "<tr name='item3_2[]'  style='display: none;'>";
					echo"<td colspan='2'>";
					
					$check = new cls_banco();
								
								$sql ="SELECT
													iteid as codigo, 
													itedescricao || ' <br> ' as descricao
										FROM 
													pse.itempergunta
										WHERE
													perid=11";
								$c = $check->carregar($sql);
								foreach($c as $linha){
									if($resid){
										$resid = $resid;
									}else{
										$resid = '0';
									}
									$check1 = "";
									$sql ="SELECT iteid FROM pse.itemselecionado WHERE resid = {$resid} AND iteid = {$linha['codigo']} ";
									$iteid = $check->pegaUm($sql);
									if ($iteid == $linha['codigo']) { $check1 = "checked=checked";}
									echo '<input type="checkbox" value="'.$linha['codigo'].'" '.$check1.' name="censo2[]" id="censo2[]"/>'.$linha['descricao'];
								}
								//$check->monta_checkbox("censo3", $sql, $objPseAnoEscolaMun->censo2, '');
							
					echo "</td>";
					echo "</tr>";
				}
			if($values["perflagboleana"] == 's' && $values["perflagitem"] == 's'  && $values["perflagmultiescolha"]== 's' && $values["pernumeroordem"]== '3.3  ')
				{
					echo "<input  type='hidden' name='idpergunta' value ={$values["perid"]}>";
					echo "<tr>";
						echo "<td>";
						
							$obResposta = new Resposta($resid,12, $empid);
							$checkNao = "";
							$checkSim = "";
							if($resid<>''){
								$obResposta->resflagresposta != "s" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
							}	
							
							echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick='validaItem(0,\"item3_3[]\")' />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick='validaItem(1,\"item3_3[]\")' />Sim";
						echo "</td>";
					echo "</tr>";
					
					echo"<tr name='item3_3[]' style='display: none;'>";
						echo "<td class='SubTituloEsquerda' colspan='2'>De que forma?</td>";
					echo "</tr>";
					
					echo "<tr name='item3_3[]'  style='display: none;'>";
					echo"<td colspan='2'>";
					
					$check = new Resposta($resid,$values["perid"], $empid);
								
								$sql ="SELECT
														iteid as codigo, 
														itedescricao || ' <br> ' as descricao
										FROM 
														pse.itempergunta
										WHERE
														perid=12"; 
								$c = $check->carregar($sql);
								foreach($c as $linha){
									if($resid){
										$resid = $resid;
									}else{
										$resid = '0';
									}
									$check1 = "";
									$sql ="SELECT iteid FROM pse.itemselecionado WHERE resid = {$resid} AND iteid = {$linha['codigo']} ";
									$iteid = $check->pegaUm($sql);
									if ($iteid == $linha['codigo']) { $check1 = "checked=checked";}
									echo '<input type="checkbox" value="'.$linha['codigo'].'" '.$check1.' name="censo3[]" id="censo3[]"/>'.$linha['descricao'];
								}
								//$check->monta_checkbox("censo3", $sql, $objPseAnoEscolaMun->censo2, '');
							
					echo "</td>";
					echo "</tr>";
				}
				if($values["perflagboleana"] == 's' && $values["perflagitem"] == 's'  && $values["perflagmultiescolha"]== 's' && $values["pernumeroordem"]== '3.4  ')
				{
					echo "<input  type='hidden' name='idpergunta' value ={$values["perid"]}>";
					echo "<tr>";
						echo "<td>";
						
							$obResposta = new Resposta($resid,13, $empid);
							$checkNao = "";
							$checkSim = "";
							if($resid<>''){
								$obResposta->resflagresposta != "s" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
							}	
						
							echo "<input type='radio' value='n' $checkNao name='per_{$values["perid"]}' onclick='validaItem(0,\"item3_4[]\")' />Não"; 
							echo "<input type='radio' value='s' $checkSim name='per_{$values["perid"]}' onclick='validaItem(1,\"item3_4[]\")' />Sim";
						echo "</td>";
					echo "</tr>";
					
					echo"<tr name='item3_4[]' style='display: none;'>";
						echo "<td class='SubTituloEsquerda' colspan='2'>De que forma?</td>";
					echo "</tr>";
					
					echo "<tr name='item3_4[]'  style='display: none;'>";
					echo"<td colspan='2'>";
					
					$check = new cls_banco();
								
								$sql ="SELECT
														iteid as codigo, 
														itedescricao || ' <br> ' as descricao
										FROM 
														pse.itempergunta
										WHERE
														perid=13"; 
								$c = $check->carregar($sql);
								foreach($c as $linha){
									if($resid){
										$resid = $resid;
									}else{
										$resid = '0';
									}
									$check1 = "";
									$sql ="SELECT iteid FROM pse.itemselecionado WHERE resid = {$resid} AND iteid = {$linha['codigo']} ";
									$iteid = $check->pegaUm($sql);
									if ($iteid == $linha['codigo']) { $check1 = "checked=checked";}
									echo '<input type="checkbox" value="'.$linha['codigo'].'" '.$check1.' name="censo4[]" id="censo4[]"/>'.$linha['descricao'];
								}
								
					echo "</td>";
					echo "</tr>";
				}
				if($values["perflagboleana"] == 'n' && $values["perflagitem"] == 's'  && $values["perflagmultiescolha"]== 's' && $values["pernumeroordem"]== '3.7. ')
				{
					echo "<input  type='hidden' name='per_{$values["perid"]}' value ={$values["perid"]}>";
					echo "<tr>";
					echo"<td colspan='2'>";
					
					$check = new cls_banco();
								
								$sql ="SELECT
														iteid as codigo, 
														itedescricao || ' <br> ' as descricao
										FROM 
														pse.itempergunta
										WHERE
														perid=18"; 
								$c = $check->carregar($sql);
								foreach($c as $linha){
									if($resid){
										$resid = $resid;
									}else{
										$resid = '0';
									}
									$check1 = "";
									$sql ="SELECT iteid FROM pse.itemselecionado WHERE resid = {$resid} AND iteid = {$linha['codigo']} ";
									$iteid = $check->pegaUm($sql);
									if ($iteid == $linha['codigo']) { $check1 = "checked=checked";}
									echo '<input type="checkbox" value="'.$linha['codigo'].'" '.$check1.' name="censo4[]" id="censo4[]"/>'.$linha['descricao'];
								}
								
					echo "</td>";
					echo "</tr>";
				}
			}
		}	
	}
	
	function carregarPerguntas2($pernumeroordem, $empid = null)
	{
		
		$sql = "SELECT * FROM pse.componente WHERE copid in ({$pernumeroordem}) ORDER BY copid";
		$rs = $this->carregar($sql);
		
		if($rs)
		{
			foreach ($rs as $values)
			{	
				if($pernumeroordem == 7){
					echo "<tr>";
					echo "<td class='SubTituloEsquerda' colspan='2' style='text-align: center'> {$values["copdescricao"]}</td>";
					echo "</tr>";
				} else {
					echo "<tr>";
					echo "<td class='SubTituloEsquerda' colspan> {$values["copdescricao"]}</td>";
					echo "<td class='SubTituloDireita' colspan> Quantitativo de Estudantes</td>";
					echo "</tr>";
				}
				if ($values["copid"]== '2') {
								$check = new cls_banco();
								
								$sql ="SELECT
														atvid as codigo, 
														atvdescricao || ' <br> ' as descricao
										FROM 
														pse.atividade
										WHERE
														copid=2"; 
								$c = $check->carregar($sql);
								foreach($c as $linha){
									if($qtdescola){
										$qtdescola = $qtdescola;
									}else{
										$qtdescola = '0';
									}
									$sql ="select qtdescola from pse.quantidadeescola where empid={$empid} and atvid= {$linha['codigo']}";
									$qtdescola = $check->pegaUm($sql);
									echo '<tr>';
									echo '<td align="left">'.$linha['descricao'].'</td>';
									echo '<td align="right"><input class="normal" type="text" maxlength="5" style="width: 13ex;" per_'.$linha['codigo'].'="" onchange="somaCampos(1);" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeypress="return somenteNumeros(event);" value="'.$qtdescola.'" maxlength="60" size="11" name="per_'.$linha['codigo'].'"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/></td>';
									//echo '<td align="right">'.campo_texto('per_'.$linha['codigo'].'','S','S','',10,60,'','','','','','per_'.$linha['codigo'].'', '', $qtdescola).'</td>';
									echo '</tr>';
								}
								echo '<tr>';
								echo '<td class="SubTituloEsquerda" colspan="2" style="text-align: right">Total <!--'.$values["copdescricao"].'--> <input class="normal" type="text" disabled style="width: 13ex;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" maxlength="60" size="11" name="total1">&nbsp&nbsp&nbsp</td>';
								echo '</tr>';
					
				}
				if ($values["copid"]== '3') {
								$check = new Resposta($resid,$values["perid"], $empid);
								
								$sql ="SELECT
														atvid as codigo, 
														atvdescricao || ' <br> ' as descricao
										FROM 
														pse.atividade
										WHERE
														copid=3"; 
								$c = $check->carregar($sql);
								foreach($c as $linha){
									if($qtdescola){
										$qtdescola = $qtdescola;
									}else{
										$qtdescola = '0';
									}
									$sql ="select qtdescola from pse.quantidadeescola where empid={$empid} and atvid= {$linha['codigo']}";
									$qtdescola = $check->pegaUm($sql);
									echo '<tr>';
									echo '<td align="left">'.$linha['descricao'].'</td>';
									echo '<td align="right"><input class="normal" type="text" maxlength="5" style="width: 13ex;" per_'.$linha['codigo'].'="" onchange="somaCampos(1);" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeypress="return somenteNumeros(event);" value="'.$qtdescola.'" maxlength="60" size="11" name="per_'.$linha['codigo'].'"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/></td>';
									//echo '<td align="right">'.campo_texto('per_'.$linha['codigo'].'','S','S','',10,60,'','','','','','per_'.$linha['codigo'].'', '', $qtdescola).'</td>';
									echo '</tr>';
								}
								echo '<tr>';
								echo '<td class="SubTituloEsquerda" colspan="2" style="text-align: right">Total <!--'.$values["copdescricao"].'--> <input class="normal" type="text" disabled style="width: 13ex;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" maxlength="60" size="11" name="total2">&nbsp&nbsp&nbsp</td>';
								echo '</tr>';
					
				}
				if ($values["copid"]== '4') {
								$check = new Resposta($resid,$values["perid"], $empid);
								
								$sql ="SELECT
														atvid as codigo, 
														atvdescricao || ' <br> ' as descricao
										FROM 
														pse.atividade
										WHERE
														copid=4"; 
								$c = $check->carregar($sql);
								foreach($c as $linha){
									if($qtdescola){
										$qtdescola = $qtdescola;
									}else{
										$qtdescola = '0';
									}
									$sql ="select qtdescola from pse.quantidadeescola where empid={$empid} and atvid= {$linha['codigo']}";
									$qtdescola = $check->pegaUm($sql);
									echo '<tr>';
									echo '<td align="left">'.$linha['descricao'].'</td>';
									echo '<td align="right"><input class="normal" type="text" maxlength="5" style="width: 13ex;" per_'.$linha['codigo'].'="" onchange="somaCampos(1);" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeypress="return somenteNumeros(event);" value="'.$qtdescola.'" maxlength="60" size="11" name="per_'.$linha['codigo'].'"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/></td>';
									//echo '<td align="right">'.campo_texto('per_'.$linha['codigo'].'','S','S','',10,60,'','','','','','per_'.$linha['codigo'].'', '', $qtdescola).'</td>';
									echo '</tr>';
								}
									echo '<tr>';
									echo '<td class="SubTituloEsquerda" colspan="2" style="text-align: right">Total <!--'.$values["copdescricao"].'--> <input class="normal" type="text" disabled style="width: 13ex;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" maxlength="60" size="11" name="total3">&nbsp&nbsp&nbsp</td>';
									echo '</tr>';
				}
				if ($values["copid"]== '5') {
								$check = new Resposta($resid,$values["perid"], $empid);
								
								$sql ="SELECT
														atvid as codigo, 
														atvdescricao || ' <br> ' as descricao
										FROM 
														pse.atividade
										WHERE
														copid=5"; 
								$c = $check->carregar($sql);
								foreach($c as $linha){
									if($qtdescola){
										$qtdescola = $qtdescola;
									}else{
										$qtdescola = '0';
									}
									$sql ="select qtdescola from pse.quantidadeescola where empid={$empid} and atvid= {$linha['codigo']}";
									$qtdescola = $check->pegaUm($sql);
									echo '<tr>';
									echo '<td align="left">'.$linha['descricao'].'</td>';
									echo '<td align="right"><input class="normal" type="text" maxlength="5" style="width: 13ex;" per_'.$linha['codigo'].'="" onchange="somaCampos(2);" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeypress="return somenteNumeros(event);" value="'.$qtdescola.'" maxlength="60" size="11" name="per_'.$linha['codigo'].'"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/></td>';
									//echo '<td align="right">'.campo_texto('per_'.$linha['codigo'].'','S','S','',10,60,'','','','','','per_'.$linha['codigo'].'', '', $qtdescola).'</td>';
									echo '</tr>';
								}
					
				}
				if ($values["copid"]== '6') {
								$check = new Resposta($resid,$values["perid"], $empid);
								
								$sql ="SELECT
														atvid as codigo, 
														atvdescricao || ' <br> ' as descricao
										FROM 
														pse.atividade
										WHERE
														copid=6"; 
								$c = $check->carregar($sql);
								foreach($c as $linha){
									if($qtdescola){
										$qtdescola = $qtdescola;
									}else{
										$qtdescola = '0';
									}
									$sql ="select qtdescola from pse.quantidadeescola where empid={$empid} and atvid= {$linha['codigo']}";
									$qtdescola = $check->pegaUm($sql);
									echo '<tr>';
									echo '<td align="left">'.$linha['descricao'].'</td>';
									echo '<td align="right"><input class="normal" type="text" maxlength="5" style="width: 13ex;" per_'.$linha['codigo'].'="" onchange="somaCampos(3);" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeypress="return somenteNumeros(event);" value="'.$qtdescola.'" maxlength="60" size="11" name="per_'.$linha['codigo'].'"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/></td>';
									//echo '<td align="right">'.campo_texto('per_'.$linha['codigo'].'','S','S','',10,60,'','','','','','per_'.$linha['codigo'].'', '', $qtdescola).'</td>';
									echo '</tr>';
								}	
				}
				if ($values["copid"]== '8') {
								$check = new Resposta($resid,$values["perid"], $empid);
								
								$sql ="SELECT
														atvid as codigo, 
														atvdescricao || ' <br> ' as descricao
										FROM 
														pse.atividade
										WHERE
														copid=8"; 
								$c = $check->carregar($sql);
								foreach($c as $linha){
									if($qtdescola){
										$qtdescola = $qtdescola;
									}else{
										$qtdescola = '0';
									}
									$sql ="select qtdescola from pse.quantidadeescola where empid={$empid} and atvid= {$linha['codigo']}";
									$qtdescola = $check->pegaUm($sql);
									echo '<tr>';
									echo '<td align="left">'.$linha['descricao'].'</td>';
									echo '<td align="right"><input class="normal" type="text" maxlength="5" style="width: 13ex;" per_'.$linha['codigo'].'="" onchange="somaCampos(4);" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeypress="return somenteNumeros(event);" value="'.$qtdescola.'" maxlength="60" size="11" name="per_'.$linha['codigo'].'"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/></td>';
									//echo '<td align="right">'.campo_texto('per_'.$linha['codigo'].'','S','S','',10,60,'','','','','','per_'.$linha['codigo'].'', '', $qtdescola).'</td>';
									echo '</tr>';
								}
				}
			}
		}
	} 
			
	
	function carregarOutrasSecretarias()
	{
		//CARREGANDO OUTRAS SECRETARIAS
		$sql= "SELECT 
				iteid as codigo,
				itedescricao as descricao 
			  FROM 
				pse.itempergunta
			  WHERE
			  	perid in ({$this->perid})";
		
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
	
	function carregaOutrasSecretariasCarregadas($resid)
	{
		$matriz = array();
		
		if($resid)
		{
			$sql = "SELECT
						itp.iteid as codigo, itp.itedescricao as descricao
					FROM
						pse.itemselecionado its
					INNER JOIN pse.resposta AS res ON res.resid = its.resid 
					INNER JOIN pse.itempergunta AS itp ON itp.iteid = its.iteid		
					WHERE
						itp.perid = {$this->perid} AND
						res.resid = {$resid} ";
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