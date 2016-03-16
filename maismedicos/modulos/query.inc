<?php 

if($_GET['manipularbanco']){	
	
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	      <link rel="stylesheet" type="text/css" href="../includes/listagem.css">';
	
	echo '<center>
			<h1>Manipular Banco de Dados</h1>
			<p><b>Banco:</b> '.$_SESSION['baselogin'].'</p>
			<form method="post" action="" name="formquery">
				<table align="center" cellpadding=10 cellspacing=10>
					<tr>
						<td>
							<p><b>Visualizar Tabelas do Esquema 1</b><br/>
							<input type="text" name="esquema1" size="25" value="'.$_REQUEST['esquema1'].'"/></p>
						</td>
						<td>
							<p><b>Visualizar Tabelas do Esquema 2</b><br/>
							<input type="text" name="esquema2" size="25" value="'.$_REQUEST['esquema2'].'"/></p>
						</td>
					</tr>
					<tr>
						<td>
							<p><b>Visualizar Colunas da Tabela 1</b><br/>
							<input type="text" name="tabela1" size="25" value="'.$_REQUEST['tabela1'].'"/></p>
						</td>
						<td>
							<p><b>Visualizar Colunas da Tabela 2</b><br/>
							<input type="text" name="tabela2" size="25" value="'.$_REQUEST['tabela2'].'"/></p>
						</td>
					</tr>
				</table>	
				
				<b>Executar Query:</b><br/>
				<textarea name="query", id="query" cols="50" rows="8">'.str_replace(array("\'"), "'", $_REQUEST['query']).'</textarea>
				<br/>
				<p><b>Criar script insert da consulta?</b><br/>
				<input type="radio" name="cria_script" value="t" '.($_REQUEST['cria_script']=='t' ? 'checked' : '').'/>&nbsp;Sim&nbsp;
				<input type="radio" name="cria_script" value="f" '.($_REQUEST['cria_script']=='f'||empty($_REQUEST['cria_script']) ? 'checked' : '').'/>&nbsp;Não
				</p>
				<p><b>Usar pg_quey sem a classe de persistência?</b><br/>
				<input type="radio" name="pg_query" value="t" '.($_REQUEST['pg_query']=='t' ? 'checked' : '').'/>&nbsp;Sim&nbsp;
				<input type="radio" name="pg_query" value="f" '.($_REQUEST['pg_query']=='f'||empty($_REQUEST['pg_query']) ? 'checked' : '').'/>&nbsp;Não
				</p>
				<p><b>Comitar?</b><br/>						
				<input type="radio" name="comitar" value="t" '.($_REQUEST['comitar']=='t' ? 'checked' : '').'/>&nbsp;Sim&nbsp;
				<input type="radio" name="comitar" value="f" '.($_REQUEST['comitar']=='f'||empty($_REQUEST['comitar']) ? 'checked' : '').'/>&nbsp;Não
				</p>
				<p><b>Mostrar Debug?</b><br/>						
				<input type="radio" name="mostra_debug" value="t" '.($_REQUEST['mostra_debug']=='t' ? 'checked' : '').'/>&nbsp;Sim&nbsp;
				<input type="radio" name="mostra_debug" value="f" '.($_REQUEST['mostra_debug']=='f'||empty($_REQUEST['mostra_debug']) ? 'checked' : '').'/>&nbsp;Não
				</p>
				<input type="submit" value="Executar" />
			</form>
		   </center>';
	
	$divBox = '<center><div style=" float:left; overflow: auto; width:350px; text-align:left; border:1px solid black; padding:5px; margin:5px; ">';
	
	$clear = '<div style="clear: both;">&nbsp;</div><p>&nbsp;</p>';
	
	$esquema1 = $_REQUEST['esquema1'];
	
	$esquema2 = $_REQUEST['esquema2'];
	
	$tabela1 = $_REQUEST['tabela1'];
	
	$tabela2 = $_REQUEST['tabela2'];
	
	$sql = $_REQUEST['query'];
	
	if($sql){
		
		$sql = str_replace("\'","'",$sql);
		
		if(strpos( strtolower(substr($sql, 0, 25)), 'select' ) !== false){
			
			echo '<p>&nbsp;</p><center><h3>Resultado da Busca</h3></center>';
						
			$rsQuery = $db->pegaLinha('select * from ( '.$sql.' ) as foo_tmp limit 1');
			
			if($rsQuery){
				$arCabecalho = array();
				foreach($rsQuery as $k => $v){
					array_push($arCabecalho, $k);
				}				
			}
			
			echo "<center><p><font color='blue'><b>Query executada:</b> {$sql}</font></p></center>";
			
			// Cria o script
			if($_REQUEST['cria_script']=='t'){
				$rsScript = $db->carregar($sql);
				$arColunas = $arCabecalho;
				if($rsScript){
					$tab = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
					$stScript = "insert into {tabela} <br/>
								{$tab}(".implode(',', $arCabecalho).")<br/> 
								 values<br/>"; 
									
					foreach($rsScript as $script){						
						$arValoresColunas = array();
						foreach($arColunas as $coluna){
							$valor = ( is_numeric($script[$coluna]) ? $script[$coluna] : ( empty($script[$coluna]) ? 'null' : "'".$script[$coluna]."'" ) ); 
							$arValoresColunas[$coluna] = $valor;
						}
						$stScript .= $tab."(".implode(',', $arValoresColunas)."),<br/>";
					}
					
					if($stScript)
						echo '<div style="padding:35px;">';
						echo '<h3>SCRIPT GERADO</h3>';
						echo '<font color="green" size="3">'.$stScript.'</font>';
						echo '</div>';
				}
			}
			
			$db->monta_lista($sql, $arCabecalho, 50, 15, '', '', '');
			
			
		}else{
			echo '<center><h3>Resultado da Execução</h3></center>';
			
			if($_REQUEST['pg_query']=='t'){
// 				$return = $db->pegaUm($sql);
				
				$con = pg_connect("host=".$GLOBALS["servidor_bd"]." port=".$GLOBALS["porta_bd"]." dbname=".$GLOBALS['nome_bd']."  user=".$GLOBALS["usuario_db"] ." password=".$GLOBALS["senha_bd"] ."");
				$sql = pg_escape_string($sql);
				
				pg_query($con, 'begin transaction; ');
				$return = pg_query($con, $sql);				
				if($_REQUEST['comitar']=='t'){
					pg_query($con, 'commit; ');
				}else{
					pg_query($con, 'rollback; ');
				}
				
			}else{
				$return = $db->executar($sql);
				if($_REQUEST['comitar']=='t') $db->commit();
			}			
			
			pg_close($con);
			echo '<center><b>Retorno da execução:</b>'.$return.'</center>';
		}
	}
	
	if($esquema1){
	
		$sqlTb = "SELECT * FROM pg_catalog.pg_tables
				WHERE schemaname IN ('{$esquema1}')
				ORDER BY schemaname, tablename";
	
		$rsTb = $db->carregar($sqlTb);
	
		echo $divBox;
		echo '<center><h3>Esquema 1: '.$esquema1.'</h3></center>';
		if($rsTb){
			foreach($rsTb as $tb){
				echo '<span style="font-size:12px;color:green;">'.$tb['tablename'].'</span><br/>';
			}
		}
		echo '<br/></div></center>';
	
	}
	
	if($esquema2){
	
		$sqlTb = "SELECT * FROM pg_catalog.pg_tables
				WHERE schemaname IN ('{$esquema2}')
				ORDER BY schemaname, tablename";
	
		$rsTb = $db->carregar($sqlTb);
	
		echo $divBox;
		echo '<center><h3>Esquema 2: '.$esquema2.'</h3></center>';
		if($rsTb){
			foreach($rsTb as $tb){
				echo '<span style="font-size:12px;color:green;">'.$tb['tablename'].'</span><br/>';
			}
		}
		echo '<br/></div></center>';
	
	}
	
	if($tabela1){
		
		$arParams = explode('.', $tabela1);
		if(count($arParams)==2){
			$esquema1 = $arParams[0];
			$tabela1 = $arParams[1];
		}else{
			$esquema1 = $esquema1 ? $esquema1 : $esquema2;
		}
		
		$sqlColum = "SELECT
						column_name,
						is_nullable,
						data_type,
						character_maximum_length
					FROM
						information_schema.columns
					WHERE
						table_schema = '{$esquema1}'
					AND
						table_name = '{$tabela1}'";
		
		$rsColum = $db->carregar($sqlColum);
		
		echo $divBox;
		echo '<center><h3>Tabela 1: '.$tabela1.'</h3></center>';
		if($rsColum){
			$html = "CREATE TABLE maismedicos.tiporegistro
						(";
			foreach($rsColum as $colum){
				echo '<span style="font-size:12px;color:blue;">'.$colum['column_name'].'</span> <span style="font-size:11px;color:gray;">('.$colum['data_type'].($colum['character_maximum_length'] ? ' '.$colum['character_maximum_length'] : '').')</span><br/>';
				
				$html .= " {$colum['column_name']} {$colum['data_type']} ({$colum['character_maximum_length']}) ".($colum['is_nullable']=='NO' ? 'NOT NULL' : '').",<br/> ";
			}
			$html .= ")
						WITH (
						  OIDS=true
						);";
		}
		
		echo '<p>'.$html.'</p>';
		
		echo '<br/></div></center>';			
	}
	
	if($tabela2){
		
		$arParams = explode('.', $tabela2);
		if(count($arParams)==2){
			$esquema2 = $arParams[0];
			$tabela2 = $arParams[1];
		}else{
			$esquema2 = $esquema1 ? $esquema1 : $esquema2;
		}
	
		$sqlColum = "SELECT
						column_name,
						is_nullable,
						data_type,
						character_maximum_length
					FROM
						information_schema.columns
					WHERE
						table_schema = '{$esquema2}'
					AND
						table_name = '{$tabela2}'";
	
		$rsColum = $db->carregar($sqlColum);
	
		echo $divBox;
		echo '<center><h3>Tabela 2: '.$tabela2.'</h3></center>';
		if($rsColum){
			foreach($rsColum as $colum){
				echo '<span style="font-size:12px;color:blue;">'.$colum['column_name'].'</span> <span style="font-size:11px;color:gray;">('.$colum['data_type'].($colum['character_maximum_length'] ? ' '.$colum['character_maximum_length'] : '').')</span><br/>';
			}
		}
		echo '<br/></div></center>';
	}
	
	echo $clear;
	
	if($_REQUEST['mostra_debug']=='t') ver($_POST);
	die;
}