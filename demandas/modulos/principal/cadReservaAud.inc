<?php
 
		//unset($_SESSION['evaid']);
		//unset($_SESSION['op']);

/**
 * RECUPERA OS DADOS DO USUÁRIO
 * Carregará o cpf em sessão	
 */
$sql = "SELECT
				usunome,
				usuemail,
				usufoneddd,
				usufonenum
			FROM
				seguranca.usuario
			WHERE
				usucpf = '".$_SESSION['usucpf']."'
			";
$dadosUsuario = $db->carregar($sql);


function inserir(){
	
	global $db;
	
	$sql = " INSERT INTO 
					demandas.eventoauditorio(
						evaevento, 
						evalocal, 
						evanumpart, 
						ungcod, 
						evastatus, 
						usucpf,
						evaprojetor, 
						evacomput, 
						evatelao, 
						evamicrofone, 
						evainternet,
						evaobs,
						evanomeresponsavel, 
						evaemailresponsavel, 
						evafonedddresponsavel, 
       					evafonenumresponsavel
					) 
					VALUES(
						 '".$_POST['evaevento']." ' ,
						 '".$_POST['evalocal']."',
						 '".$_POST['evanumpart']."',
						 '".$_POST['ungcod']."',
						 'A',
			  			 '".$_SESSION['usucpf']."',
						 '".$_POST['evaprojetor']."',
						 '".$_POST['evacomput']."',
						 '".$_POST['evatelao']."',
						 '".$_POST['evamicrofone']."',
						 '".$_POST['evainternet']."',
						 '".$_POST['evaobs']."',
						 '".$_POST['evanomeresponsavel']."',
						 '".$_POST['evaemailresponsavel']."',
						 '".$_POST['evafonedddresponsavel']."',
						 '".$_POST['evafonenumresponsavel']."' 
						 
					)
					RETURNING 
			 			 evaid
			";
	$evaid = $db->pegaUm($sql);
	
	
	if($_POST['datax']){
		
		$msgx = array();
		$msgx2 = "";
		
		for($i=0; $i<count($_POST['datax']); $i++){
		
			if($_POST['turnox'][$i] == 'Manhã'){
				$campoaga = "aga_evaid_manha";
			}
			elseif($_POST['turnox'][$i] == 'Almoço'){
				$campoaga = "aga_evaid_almoco";
			}
			elseif($_POST['turnox'][$i] == 'Tarde'){
				$campoaga = "aga_evaid_tarde";
			}
			
			$sqle = "select agaid 
					 from demandas.agendaauditorio 
					 where $campoaga is not null 
					 and agastatus = 'A'
					 and agadata = '".formata_data_sql($_POST['datax'][$i])."'";
			$existe = $db->pegaUm($sqle);
			
			if(!$existe){
				$sql2 = " INSERT INTO demandas.agendaauditorio(agadata, agastatus, $campoaga) 
						 VALUES('".formata_data_sql($_POST['datax'][$i])."','A',".$evaid.") 
					   ";	
				$db->executar($sql2);
			}
			else{
				array_push($msgx, $_POST['datax'][$i] ." ". $_POST['turnox'][$i]);
			}
					
		}
		
		if($msgx[0]){
			
			$msgx2 = "Não foi possível reservar a(s) data(s) abaixo: \n";
			for($i=0; $i<count($msgx); $i++){
				$msgx2 .= " - " . $msgx[$i] . " \n";  
			}
			$msgx2 .= "Porque já foi(ram) reservada(s) por outro usuário! \n\n";
			$msgx2 .= "A(s) data(s) que não foi(ram) listada(s), foi(ram) cadastrada(s) com sucesso!";
		}
		
	}
	

	$db->commit();
	
	//envia email
	enviaEmailReservaAud($evaid, 'I');
	
	return $msgx2;
	//header( "Location: demandas.php?modulo=principal/cadReservaAud&acao=A" );
	//exit();
}


function selecionar(){
	global $db;
	$sql = "SELECT  
					e.evaid, 
					e.evaevento, 
					e.evalocal, 
					e.evanumpart, 
					e.ungcod, 
					e.evastatus, 
					e.usucpf, 
		            e.evaprojetor, 
		            e.evacomput, 
		            e.evatelao, 
		            e.evamicrofone, 
		            e.evainternet, 
		            e.evaobs,
		            e.evanomeresponsavel, 
		            e.evaemailresponsavel, 
		            e.evafonedddresponsavel, 
       				e.evafonenumresponsavel
			FROM  
					demandas.eventoauditorio e 
			WHERE 
					e.evaid = '".$_SESSION['evaid']."'
			";
	return $db->carregar($sql);
}


function alterar(){
	global $db;
	$sql = "UPDATE 
				demandas.eventoauditorio SET 
				evaevento 			  = '".$_POST['evaevento']."',
				evalocal 			  = '".$_POST['evalocal']."',
				evanumpart 			  =  ".$_POST['evanumpart'].",
				ungcod 				  =  ".$_POST['ungcod'].",
				evaprojetor 		  = '".$_POST['evaprojetor']."',
				evacomput 		      = '".$_POST['evacomput']."',
				evatelao              = '".$_POST['evatelao']."',
				evamicrofone          = '".$_POST['evamicrofone']."',
				evainternet           = '".$_POST['evainternet']."',
				evaobs 				  = '".$_POST['evaobs']."',	
				evanomeresponsavel 	  = '".$_POST['evanomeresponsavel']."',
				evaemailresponsavel	  = '".$_POST['evaemailresponsavel']."',
				evafonedddresponsavel = '".$_POST['evafonedddresponsavel']."',
				evafonenumresponsavel = '".$_POST['evafonenumresponsavel']."' 
			WHERE 
				evaid 				  = '".$_POST['evaid']."'
			";
	$db->executar($sql);
	
	
	$sql2 = " UPDATE demandas.agendaauditorio SET
			  agastatus = 'I'
			  WHERE aga_evaid_manha  = ".$_POST['evaid']."
			  	 OR aga_evaid_almoco = ".$_POST['evaid']."
			  	 OR aga_evaid_tarde  = ".$_POST['evaid']."
		    ";	
	$db->executar($sql2);

	
	
	if($_POST['datax']){
	
		$msgx = array();
		$msgx2 = "";
		
		for($i=0; $i<count($_POST['datax']); $i++){
		
			if($_POST['turnox'][$i] == 'Manhã'){
				$campoaga = "aga_evaid_manha";
			}
			elseif($_POST['turnox'][$i] == 'Almoço'){
				$campoaga = "aga_evaid_almoco";
			}
			elseif($_POST['turnox'][$i] == 'Tarde'){
				$campoaga = "aga_evaid_tarde";
			}
			
			$sqle = "select agaid 
					 from demandas.agendaauditorio 
					 where $campoaga is not null 
					 and agastatus = 'A'
					 and agadata = '".formata_data_sql($_POST['datax'][$i])."'";
			$existe = $db->pegaUm($sqle);
			
			if(!$existe){
				$sql2 = " INSERT INTO demandas.agendaauditorio(agadata, agastatus, $campoaga) 
						 VALUES('".formata_data_sql($_POST['datax'][$i])."','A',".$_POST['evaid'].") 
					   ";	
				$db->executar($sql2);
			}
			else{
				array_push($msgx, $_POST['datax'][$i] ." ". $_POST['turnox'][$i]);
			}
			
		}
		
		if($msgx[0]){
			
			$msgx2 = "Não foi possível reservar a(s) data(s) abaixo: \n";
			for($i=0; $i<count($msgx); $i++){
				$msgx2 .= " - " . $msgx[$i] . " \n";  
			}
			$msgx2 .= "Porque já foi(ram) reservada(s) por outro usuário! \n\n";
			$msgx2 .= "A(s) data(s) que não foi(ram) listada(s), foi(ram) cadastrada(s) com sucesso!";
		}
				
	}	
	
	//exit;
	$db->commit();
	
	//envia email
	enviaEmailReservaAud($_POST['evaid'], 'A');
	
	return $msgx2;
	
}

function deletar(){
	global $db;
	
	$sql = "UPDATE demandas.eventoauditorio SET 
			evastatus = 'I' 
			WHERE evaid = ".$_SESSION['evaid'];
	$db->executar($sql);
	
	$sql2 = " UPDATE demandas.agendaauditorio SET
			  agastatus = 'I'
			  WHERE aga_evaid_manha = ".$_SESSION['evaid']."
			  	 OR aga_evaid_almoco = ".$_SESSION['evaid']."
			  	 OR aga_evaid_tarde = ".$_SESSION['evaid']."
		    ";	
	$db->executar($sql2);
	
	$db->commit();
	
	//envia email
	enviaEmailReservaAud($_SESSION['evaid'], 'C');
}

function mudaLocal($evaid){
	global $db;

	$sql2 = " UPDATE demandas.agendaauditorio SET
			  agastatus = 'I'
			  WHERE aga_evaid_manha = ".$evaid."
			  	 OR aga_evaid_almoco = ".$evaid."
			  	 OR aga_evaid_tarde = ".$evaid."
		    ";	
	$db->executar($sql2);
	$db->commit();
	
	echo $evaid;
	exit;
}


if($_POST['evaidLocalAjax']){
	mudaLocal($_POST['evaidLocalAjax']);
	exit;
}


if($_POST['evaevento'] && $_POST['evalocal'] && $_POST['ungcod'] && $_POST['evanumpart'] && !$_POST['evaid'] && $_POST['evanomeresponsavel']){
	$msgx = inserir();
	if(!$msgx){
		print "<script>alert('Operação realizada com sucesso!');</script>";
	}
	else{
		?>
			<script type="text/javascript">
			  alert("<?php echo preg_replace("/\r?\n/", "\\n", addslashes($msgx)); ?>");
			</script>
		<?
	}
	print "<script>location.href='demandas.php?modulo=principal/cadReservaAud&acao=A';</script>";
	//header( "Location: demandas.php?modulo=principal/cadReservaAud&acao=A" );
	exit();
	
}

if($_POST['evaevento'] && $_POST['evalocal'] && $_POST['ungcod'] && $_POST['evanumpart'] && $_POST['evaid'] && $_POST['evanomeresponsavel']){
	$msgx = alterar();
	unset($_SESSION['evaid']);
	if(!$msgx){
		print "<script>alert('Operação realizada com sucesso!');</script>";
	}
	else{
		?>
			<script type="text/javascript">
			  alert("<?php echo preg_replace("/\r?\n/", "\\n", addslashes($msgx)); ?>");
			</script>
		<?
	}
	print "<script>location.href='demandas.php?modulo=principal/cadReservaAud&acao=A';</script>";
	//header( "Location: demandas.php?modulo=principal/cadReservaAud&acao=A" );
	exit();
}


if($_REQUEST['evaid'] && $_REQUEST['op']){
	session_start();
	$_SESSION['evaid'] = $_REQUEST['evaid'];
	$_SESSION['op'] = $_REQUEST['op'];
	header( "Location: demandas.php?modulo=principal/cadReservaAud&acao=A" );
	exit();
}


if($_SESSION['evaid'] && $_SESSION['op']){

	if($_SESSION['op'] == 'delete'){
		deletar();	
		unset($_SESSION['evaid']);
		unset($_SESSION['op']);
	}
	if($_SESSION['op'] == 'update'){
		$dados = selecionar();	
		unset($_SESSION['evaid']);
		unset($_SESSION['op']);
		extract($dados[0]);
	}
	
}




include APPRAIZ ."includes/cabecalho.inc";
print '<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Reserva de Auditório', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>

<script type="text/javascript" src="../includes/prototype.js"></script>

  
<body>

 
<form id="formR" name="formR" action="" method="post" onsubmit="return validaForm();" >

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<input type="hidden" name="evaid" id="evaid" value="<?=$evaid?>">
	
	<tr>
		<td align="right" class="SubTituloDireita" >Local:</td>
		<td>
			<input type="radio" name="evalocal" value="S" <?if($evalocal == 'S') echo 'checked';?> onclick="mudaLocal();"> <b>Sede</b> (110 cadeiras fixas)
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" name="evalocal" value="A" <?if($evalocal == 'A') echo 'checked';?> onclick="mudaLocal();"> <b>Anexo</b> (109 cadeiras fixas)
			<?= obrigatorio(); ?>
		</td>
	</tr>
	<tr>	
		<td width="33%" align="right" class="SubTituloDireita" >Agenda:</td>
		<td align="left">
			<a href="#" onclick="popupAgenda();"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir">&nbsp;&nbsp;Inserir Data/Turno</a>
			<br>
			<table id="tabela_agenda" width="70%" align="left" border="0" cellspacing="2" cellpadding="2" class="listagem">
				<thead>
					<tr id="cabecalho">
						<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
						<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data</strong></td>
						<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Turno</strong></td>
					</tr>
				</thead>				
				<tbody>
					<?php
						if ($evaid){
						
							$sql = pg_query("
								SELECT  		    
								    to_char(a.agadata::date,'DD/MM/YYYY') as datax,
							            CASE 
							            	WHEN aga_evaid_manha is not null THEN 'Manhã'
							            	WHEN aga_evaid_almoco is not null THEN 'Almoço'
							            	WHEN aga_evaid_tarde is not null THEN 'Tarde'
							            END as turnox,
							            CASE 
							            	WHEN aga_evaid_manha is not null THEN '0' 
							            	WHEN aga_evaid_almoco is not null THEN '1'
							            	WHEN aga_evaid_tarde is not null THEN '2'
							            END || to_char(a.agadata::date,'YYYYMMDD') as id
								FROM  demandas.eventoauditorio e  
								LEFT JOIN demandas.agendaauditorio a ON (e.evaid=a.aga_evaid_manha OR e.evaid=a.aga_evaid_almoco OR e.evaid=a.aga_evaid_tarde)
								WHERE a.agastatus='A' and e.evaid = {$evaid} 
								ORDER BY 1");
	
							$count = 1;
							
							while (($dados = pg_fetch_array($sql)) != false){
								
								$id = $dados["id"];
								$datax = $dados["datax"];
								$turnox = $dados["turnox"];
								
								$habilitado = '1';
								if ($habilitado){
									$excluir_modulo = "<img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir' onclick=\"excluiItem('". $id ."');\">";
								}else{
									$excluir_modulo = "<img src='/imagens/excluir_01.gif' style='cursor:pointer;' border='0'/>";
								}
								
								$cor = "#f4f4f4";
								$count++;
								if ($count % 2){
									$cor = "#e0e0e0";
								}
								
								echo "
									<tr id=\"linha_".$id."\" bgcolor=\"" . $cor . "\">
										<td align=\"center\">
											" . $excluir_modulo . "
										</td>
										<td align='center'>" . $datax . "<input type='hidden' id='datax[" . $id . "]' name='datax[]' value='" . $datax . "'></td>
										<td align='center'>" . $turnox . "<input type='hidden' id='turnox[" . $id . "]' name='turnox[]' value='" . $turnox . "'></td>
									</tr>";
							}
						}
					?>
				</tbody>
			</table>
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" >Nome do Evento:</td>
		<td>
			<?= campo_texto( 'evaevento', 'N', 'S', '', 80, 100, '', '','','','','',''); echo obrigatorio();?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Órgão/Secretaria:</td>
		<td>
			<?php
				$sql = " SELECT
						  ungcod AS codigo, 
					  	  upper(ungabrev)||' - '||ungdsc as descricao 
						 FROM
						  public.unidadegestora 
						 WHERE
						  ungstatus = 'A'
						  order by ungabrev, ungdsc";
				$db->monta_combo("ungcod",$sql,'S',"-- Selecione um setor --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
		
	<tr>
		<td align='right' class="SubTituloDireita">Nº de Participantes:</td>
		<td><?= campo_texto( 'evanumpart', 'S', 'S', '', 7, 6, '######', ''); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Equipamentos Necessários:<br></td>
		<td>
			<input type="checkbox" name="evaprojetor" value="S" <?if($evaprojetor == 'S') echo 'checked';?>> Projetor
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="checkbox" name="evatelao" value="S" <?if($evatelao == 'S') echo 'checked';?>> Telão
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="checkbox" name="evacomput" value="S" <?if($evacomput == 'S') echo 'checked';?>> Computador para Projeção
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="checkbox" name="evamicrofone" value="S" <?if($evamicrofone == 'S') echo 'checked';?>> Microfones
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="checkbox" name="evainternet" value="S" <?if($evainternet == 'S') echo 'checked';?>> Acesso a Internet
		</td>
	</tr>				
	<tr>
		<td align='right' class="SubTituloDireita" valign="top"><b>Observações:</td>
		<td><?= campo_textarea('evaobs', 'N', 'S', '', 80, 5, 500); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome do Responsável:</td>
		<td>
			<? if($evanomeresponsavel == ''){$evanomeresponsavel  = $dadosUsuario[0]["usunome"];}?>
			<?= campo_texto( 'evanomeresponsavel', 'S', 'S', '', 50, 50, '', '' ); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">E-Mail do Responsável:</td>
		<td>
			<? if($evaemailresponsavel ==''){$evaemailresponsavel = $dadosUsuario[0]["usuemail"];}?>
			<?= campo_texto( 'evaemailresponsavel', 'S', 'S', '', 50, 100, '', '' ); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">Telefone (DDD) + Telefone do Responsável:</td>
		<td>
			<? if($evafonedddresponsavel == '' && $evafonenumresponsavel ==''){
					$evafonedddresponsavel=$dadosUsuario[0]["usufoneddd"];
					$evafonenumresponsavel = $dadosUsuario[0]["usufonenum"];  } ?>
			<?= campo_texto( 'evafonedddresponsavel', 'S', 'S', '', 3, 2, '##', '' ); ?>
			<?= campo_texto( 'evafonenumresponsavel', 'S', 'S', '', 18, 15, '###-####|####-####', '' ); ?>
		</td>
	</tr>
	
	<tr>
		<td align='right' class="SubTituloDireita" valign="top"><b>Informações Importantes:</b></td>
		<td> 

			1) Qualquer evento programado para esse ambiente deverá ser realocado, a cargo da secretaria/órgão, caso haja requisição do espaço pelo Gabinete do Ministro.<br>
			2) Está sob a responsabilidade de cada Órgão/Secretaria (caso necessário):<br>
			   &nbsp;&nbsp;&nbsp;&nbsp; - mestre de ceminônias;<br>
			   &nbsp;&nbsp;&nbsp;&nbsp; - cerimonial;<br>
			   &nbsp;&nbsp;&nbsp;&nbsp; - serviço de copa e apoio;<br> 
			   &nbsp;&nbsp;&nbsp;&nbsp; - organização de mobiliário.<br>
			3) A utilização do hall de entrada privativa deve ser comunicada com a devida justificativa.<br>
			4) Evite transtornos: em caso de cancelamento do evento, comunique este Setor com urgência pelo telefone (61) 2022-7555 ou pelo e-mail auditorio@mec.gov.br.

		</td>
	</tr>										
	<tr bgcolor="#C0C0C0">
				<td></td>
				<td>
				<input type="submit" class="botao" name="btalterar" value="Salvar"> 
				<?
					if($evaid){
						?>
						<input type="button" class="botao" name="btnovo" value="Novo" onclick="location.href='demandas.php?modulo=principal/cadReservaAud&acao=A';"> 
						<?
					}
				?>
				</td>
			</tr>
	<tr>
	<tr>
		<td align=center colspan=2><b>Minhas Reservas</b></td>
	</tr>	
	
	</table>

	</form>
	
	<?php
	$perfil = arrayPerfil();

	if( in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) ){
	
			$sql = "SELECT
						'<center><a href=\"demandas.php?modulo=principal/cadReservaAud&acao=A&evaid=' || e.evaid || '&op=update\"><img border=0 src=\"../imagens/alterar.gif\" title=\"Editar Evento\" /></a> <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=principal/cadReservaAud&acao=A&evaid=' || e.evaid || '&op=delete\');\" ><img border=0 src=\"../imagens/excluir.gif\" title=\"Cancelar Evento\" /></a></center>' as acao, 
						evaevento as nome,
						upper(ungabrev)||' - '||ungdsc as orgao,
						e.evanomeresponsavel,
						'('||e.evafonedddresponsavel||') '||e.evafonenumresponsavel AS DDDTelefone 
		 			FROM 
		 				demandas.eventoauditorio e
					INNER JOIN 
			 			public.unidadegestora u on u.ungcod = e.ungcod
			 		LEFT JOIN 
			 			seguranca.usuario usu ON usu.usucpf = e.usucpf
					WHERE 
						evastatus = 'A'
					ORDER BY 
			 			evaid";
			//dbg($sql,1);
			//$cabecalho = array( "<center>Ação</center>" , "Nome do Evento", "Período", "Órgão/Secretaria");
			$cabecalho = array( "<center>Ação</center>" , "Nome do Evento", "Órgão/Secretaria", "Responsável", "Telefone para Contato");
			$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
			
	}
	else{
		if($evanomeresponsavel != nomeUser($_SESSION['usucpf'] )){
			$select = ",e.evanomeresponsavel,'('||e.evafonedddresponsavel||')'||e.evafonenumresponsavel";
			}
			$sql = "SELECT
			'<center><a href=\"demandas.php?modulo=principal/cadReservaAud&acao=A&evaid=' || e.evaid || '&op=update\"><img border=0 src=\"../imagens/alterar.gif\" title=\"Editar Evento\" /></a> <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=principal/cadReservaAud&acao=A&evaid=' || e.evaid || '&op=delete\');\" ><img border=0 src=\"../imagens/excluir.gif\" title=\"Cancelar Evento\" /></a></center>' as acao, 
			evaevento as nome,
			upper(ungabrev)||' - '||ungdsc as orgao
			".$select."
			 FROM demandas.eventoauditorio e
			 inner join public.unidadegestora u on u.ungcod = e.ungcod
			WHERE evastatus = 'A'
			and usucpf = '".$_SESSION['usucpf']."'
			ORDER BY 
			 evaid";
			//dbg($sql,1);
			//$cabecalho = array( "<center>Ação</center>" , "Nome do Evento", "Período", "Órgão/Secretaria");
			if($evanomeresponsavel != nomeUser($_SESSION['usucpf'] )){
				$cabecalho = array( "<center>Ação</center>" , "Nome do Evento", "Órgão/Secretaria","Responsável", "Telefone para Contato");
			}else{
				$cabecalho = array( "<center>Ação</center>" , "Nome do Evento", "Órgão/Secretaria");
			}
			$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
			
	}

?>
	
<script type="text/javascript">

d = document;

function validaForm(){
	
	if(d.formR.evalocal[0].checked == false && document.formR.evalocal[1].checked == false){
		alert ('O campo Local deve ser preenchido.');
		d.formR.evalocal[0].focus();
		return false;
	}
	
	var tabela = d.getElementById("tabela_agenda");
	if(tabela.rows.length < 2){
		alert ('O campo Agenda deve ser preenchido.');
		return false;
	}

	if(d.formR.evaevento.value == ''){
		alert ('O campo Nome do Evento deve ser preenchido.');
		d.formR.evaevento.focus();
		return false;
	}
	
	if(d.formR.ungcod.value == ''){
		alert ('O campo Órgão/Secretaria deve ser preenchido.');
		d.formR.ungcod.focus();
		return false;
	}
	if(d.formR.evanumpart.value == ''){
		alert ('O campo Nº de Participantes deve ser preenchido.');
		d.formR.evanumpart.focus();
		return false;
	}
	if(d.formR.evanomeresponsavel.value == ''){
		alert ('O campo Nome do Responsável deve ser preenchido.');
		d.formR.evanomeresponsavel.focus();
		return false;
	}
	if(d.formR.evaemailresponsavel.value == ''){
		alert ('O campo E-Mail do Responsável deve ser preenchido.');
		d.formR.evaemailresponsavel.focus();
		return false;
	}
	if ( !(/^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/.test(d.formR.evaemailresponsavel.value)) ) {
		alert("O E-Mail do Responsável está incorreto.");
		d.formR.evaemailresponsavel.focus();
		return false;
	}
	if(d.formR.evafonedddresponsavel.value == ''){
		alert ('O campo Telefone (DDD) deve ser preenchido.');
		d.formR.evafonedddresponsavel.focus();
		return false;
	}
	if(d.formR.evafonenumresponsavel.value == ''){
		alert ('O campo Telefone do Responsável deve ser preenchido.');
		d.formR.evafonenumresponsavel.focus();
		return false;
	}
	
	
}

function exclusao(url) {
	var questao = confirm("Deseja realmente cancelar este Evento?");
	if (questao){
		window.location = url;
	}
}

function popupAgenda(){
	
	var evalocal = '';
	if(d.formR.evalocal[0].checked == false && d.formR.evalocal[1].checked == false){
		alert ('O campo Local deve ser preenchido.');
		document.formR.evalocal[0].focus();
		return false;
	}
	
	if(d.formR.evalocal[0].checked == true) evalocal = d.formR.evalocal[0].value;
	if(d.formR.evalocal[1].checked == true) evalocal = d.formR.evalocal[1].value;
	
	param = 'evalocal='+evalocal+'&';
	param += d.getElementById('evaid').value ? 'evaid='+d.getElementById('evaid').value : ''; 
	
	w = window.open('?modulo=principal/agendaReservaAud&acao=A&'+param,'pesqAgenda','scrollbars=1,location=0,toolbar=0,menubar=0,status=0,titlebar=0,directories=0,width=970,height=500,left=250,top=125'); 
	w.focus();	
}


function excluiItem(id) {		
	var linha = d.getElementById("linha_"+id).rowIndex;
	var tabela = d.getElementById("tabela_agenda");
	tabela.deleteRow(linha);
}

function mudaLocal(){
	var id = d.getElementById("evaid").value;
	var local = "<?=$evalocal?>";
	if(id){
		
		if(confirm('A mudança de local, irá apagar toda agenda deste evento. \nDeseja realizar esta operação?')){
			mudaLocal2(id);
		}
		else{
			if(local == 'S') d.formR.evalocal[0].checked = true;
			if(local == 'A') d.formR.evalocal[1].checked = true;
		}
		
	}
	
}

function mudaLocal2(idx)
{					  
		var req = new Ajax.Request('demandas.php?modulo=principal/cadReservaAud&acao=A', {
				        method:     'post',
				        parameters: '&evaidLocalAjax=' + idx,
				        onComplete: function (res)
				        {	
				        	if(res.responseText){
				        		alert("Operação Efetuada com sucesso.");
				        		location.href="demandas.php?modulo=principal/cadReservaAud&acao=A&op=update&evaid=" + res.responseText;
				        	}
				        	else{
				        		alert("Erro. Contate o administrador do sistema.");
				        	}

				        }
				  });								  

}

</script>
</body>

 

