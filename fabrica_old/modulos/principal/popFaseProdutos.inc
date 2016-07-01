<?php

/*** Inclue os arquivos necessários ***/
//include_once "config.inc";
$banco = new cls_banco();
//$servicoFaseProdutoRepositorio = new ServicoFaseProdutoRepositorio();


$ansid = $_REQUEST['ansid'];
$dspid = $_REQUEST['dspid'];
$tpeid = $_REQUEST['tpeid'];

//FUNÇÕES DEFINIDAS NA PÁGINA PARA RESOLVER PROBLEMA DE COMMIT.
//TODAS AS FUNCOES SÂO MÉTODOS DE SERVICO_FASE_PRODTO_REPOSITORIO
	function recuperaFaseDisciplinaProduto( $idFaseDisciplinaProduto, cls_banco $banco ){
		$sql = "SELECT fdp.fdpid, fdp.prdid, fdp.fsdid 
				FROM fabrica.fasedisciplinaproduto fdp where fdpid = $idFaseDisciplinaProduto";
		return $banco->pegaLinha($sql);		
	}

	function verificaSeFoiAuditado( $idServicoFaseProduto, cls_banco $banco ){
		$sql = "SELECT count(*) FROM fabrica.detalhesauditoria WHERE sfpid = $idServicoFaseProduto";
		return (bool) $banco->pegaUm($sql);
	}
	
	function recupereServicoFaseProdutoPorId( $idServicoFaseProduto, cls_banco $banco ){
		$sql = "SELECT sfp.sfpid, sfp.fdpid, fdp.prdid, fdp.fsdid
				from fabrica.servicofaseproduto sfp
				join fabrica.fasedisciplinaproduto fdp
					on fdp.fdpid = sfp.fdpid
				where sfp.sfpid = $idServicoFaseProduto";
		return $banco->pegaLinha($sql);
	}
	
	function verificaSeHaArtefato( $idAnaliseSolicitacao ){
		$sql = "SELECT sfpid from fabrica.servicofaseproduto where fdpid = $idFaseDisciplinaProduto 
				and ansid = $idAnaliseSolicitacao and tpeid = $idTipoExecucao";
		$linha = $banco->pegaLinha($sql);
		return $linha['sfpid'];
	}
	
	function recuperePorId($idServicoFaseProduto, cls_banco $banco){
		$sql = "select 
				
				p.prddsc as produto
				
				from fabrica.servicofaseproduto sfp
				join fabrica.fasedisciplinaproduto fdp
					on sfp.fdpid = fdp.fdpid
				inner join fabrica.produto p
					on p.prdid = fdp.prdid AND p.prdid <> 44 
				join fabrica.fasedisciplina fd
					on fd.fsdid = fdp.fsdid
				join fabrica.fase f
					on f.fasid = fd.fasid
				join fabrica.disciplina d
					on d.dspid = fd.dspid
					
				where sfp.sfpid = $idServicoFaseProduto";
		return $banco->pegaUm($sql);
	}
	
/*** Se os dados da lista foram submetidos... ***/
if( $_POST['varaux'] )
{
	
	//deleta produtos
	$sql = "SELECT sp.sfpid FROM fabrica.servicofaseproduto sp
			INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
			INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
			WHERE sp.ansid = {$ansid} 
			and sp.tpeid = {$tpeid} 
			and fd.dspid = {$dspid}
			and fdp.fdpstatus = 'A'
			order by 1";
	$sfpid = $db->carregarColuna($sql);
	
	$listaFaseDisciplinaComArtefatosAuditados = array();
	
	foreach ($sfpid as $idServicoFaseProduto){
		if (verificaSeFoiAuditado($idServicoFaseProduto, $banco)){
			$listaProdutoAuditado[] = recuperePorId($idServicoFaseProduto, $banco);
			$servicoFaseProduto = recupereServicoFaseProdutoPorId($idServicoFaseProduto, $banco);
			$listaFaseDisciplinaComArtefatosAuditados[] = $servicoFaseProduto['fsdid'];
		} else {
			$listaServicoFaseProdutoParaDeletar[] = $idServicoFaseProduto;
		}
	}
	
	if (count($listaProdutoAuditado)>0){
		$alertJavaScript  = "<script type=\"text/javascript\">";
		$alertJavaScript .= 	"alert('O(s) artefato(s) não será(ão) removido(s) devido auditoria realizada: ";
		$alertJavaScript .= 		implode(", ", $listaProdutoAuditado);
		$alertJavaScript .= 	" ');";
		$alertJavaScript .= "</script>";
		print $alertJavaScript;
	}
	
	if($listaServicoFaseProdutoParaDeletar) {
		$sql = "DELETE FROM fabrica.servicofaseproduto WHERE sfpid in (".implode(',',$listaServicoFaseProdutoParaDeletar).")";
		$db->executar($sql);
	}
	
	if($_POST['fdpid']) {
		
		foreach($_POST['fdpid'] as $fdpid) {
			$fdp = recuperaFaseDisciplinaProduto( $fdpid, $banco );
			if (!in_array($fdp['fsdid'], $listaFaseDisciplinaComArtefatosAuditados)){
				$sql = "INSERT INTO fabrica.servicofaseproduto(fdpid, ansid, tpeid) VALUES ({$fdpid}, {$ansid}, {$tpeid})";
			}
			$db->executar($sql);
			
//			if ($sfp['fsdid'] != $fdp['fsdid']){
//				$sql = "INSERT INTO fabrica.servicofaseproduto(fdpid, ansid, tpeid) VALUES ({$fdpid}, {$ansid}, {$tpeid})";
//			}
//			$db->executar($sql);
		}
	}
	
	$db->commit();
	
	
	
	$sql = "SELECT distinct f.fasid, f.fasdsc 
			FROM fabrica.servicofaseproduto sp 
			INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
			INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
			INNER JOIN fabrica.fase f ON f.fasid = fd.fasid
			WHERE sp.ansid = {$ansid} and fdp.fdpstatus = 'A'
			order by 1";
	$fase = $db->carregar($sql);

	$html = '';
	
	if($fase) {
		
		for($i=0;$i<=count($fase)-1;$i++){
			
			$sql = "SELECT p.prddsc 
					FROM fabrica.servicofaseproduto sp 
					INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
					INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
					INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
					WHERE sp.ansid = {$ansid} 
					and fd.fasid = {$fase[$i]['fasid']}
					and fd.dspid = {$dspid}
					and sp.tpeid = {$tpeid}
					and fdp.fdpstatus = 'A'
					order by 1";
			$produto = $db->carregarColuna($sql);
	
			if($produto){
				$html .= "<b>{$fase[$i]['fasdsc']}</b><br/>";
				foreach($produto as $p) {
					 $html .= "<span style=padding-left:20px><img src=../imagens/seta_filho.gif align=absmiddle> ".$p."</span><br/>";
				}
			}
			
		}
		
	}
		
	echo '<script>';
	
	if($html) {
		echo 'window.opener.$("#botaonao_'.$dspid.'_'.$tpeid.'").hide();';
		echo 'window.opener.$("#botaosim_'.$dspid.'_'.$tpeid.'").show();';
	}
	else{
		echo 'window.opener.$("#botaosim_'.$dspid.'_'.$tpeid.'").hide();';
		echo 'window.opener.$("#botaonao_'.$dspid.'_'.$tpeid.'").show();';
	} 
	
	
	echo '	window.opener.$("#'.$dspid.'_'.$tpeid.'").html(\''.$html.'\');
			//alert("Dados gravados com sucesso.");
			self.close();
		  </script>';
	die;
}



?>

<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title><?= $titulo ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">

		<form method="post" name="formPopLista" id="formPopLista" action="">
		
			<input type="hidden" name="dspid" value="<?php echo $dspid; ?>">
			<input type="hidden" name="tpeid" value="<?php echo $tpeid; ?>">
			<input type="hidden" name="ansid" value="<?php echo $ansid; ?>">
			<input type="hidden" name="varaux" value="">
		

			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<tr bgcolor="#cdcdcd">
					<td style="text-align:center;" colspan="2">
						<?php $sql = "SELECT dspdsc	FROM fabrica.disciplina WHERE dspid = ".$dspid;
						$dscdisciplina = $db->pegaUm($sql);
						?>
						<b>
							DISCIPLINA: <?=$dscdisciplina?> - <?echo ($tpeid == '1' ? 'Contratada' : 'Contratante');?>
							<br>
							FASES 
						</b>
					</td>
				</tr>
				<tr>
					<td style="text-align: center;" bgcolor="#dcdcdc"><B>CONCEPÇÃO</B></td>
					<td>
					<?
					$btnSalvar = false;
					$fase = 1;
					//verifica se esta checado com o contratante ou contratada
					if($tpeid == '1') $tpeidAux = '2';
					else $tpeidAux = '1';
					
					$verificaTipo = "";
					$sql = "SELECT count(sp.fdpid) as total 
							FROM fabrica.servicofaseproduto sp 
							INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
							INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
							INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
							WHERE sp.ansid = {$ansid} 
							and fd.fasid = {$fase}
							and fd.dspid = {$dspid}
							and sp.tpeid = {$tpeidAux}
							and fdp.fdpstatus = 'A'";
					$total = $db->pegaUm($sql);
					if($total > 0) $verificaTipo = "disabled";		
					
					$sql = "SELECT
										f.fdpid AS codigo,
										p.prddsc AS descricao
									FROM
										fabrica.fasedisciplinaproduto f
									INNER JOIN
										fabrica.fasedisciplina fd ON fd.fsdid = f.fsdid
									INNER JOIN
										 fabrica.produto p ON p.prdid = f.prdid
									WHERE
										p.prdstatus='A' 
										and f.fdpstatus = 'A'
										AND fd.fasid = {$fase}
										AND fd.dspid = ".$dspid."
										order by 2";
							//dbg($sql,1);
					$dados = $db->carregar($sql);
					$i=0;
					if($dados){
						
						echo '<input type="checkbox" class="marcatodos" name="todos_'.$fase.'" id="todos_'.$fase.'" '.$verificaTipo.' value="" onclick="marcaTodos('.$fase.',this)">&nbsp;Todos<br>';
						echo '<div id="divMarcaTodos_'.$fase.'">';
						
						foreach($dados as $dado){
									
									//verifica se esta checado
							$checadoSim = "";
							$sql = "SELECT sp.fdpid 
											FROM fabrica.servicofaseproduto sp 
											INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
											INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
											INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
											WHERE sp.ansid = {$ansid} 
											and fdp.fdpstatus = 'A'
											and fd.fasid = {$fase}
											and fd.dspid = {$dspid}
											and sp.tpeid = {$tpeid}
											and sp.fdpid = {$dado['codigo']}";
							$checado = $db->pegaUm($sql);
							
							if($checado) $checadoSim = "checked";

                                                        if($dados[$i]['descricao'] == 'Serviço sem necessidade de artefato'){
                                                            $classDesmarca = 'desmarcar';
                                                            $onclickDesmarca = "desmarcaTodos('$fase',this)";
                                                        }else{
                                                            $classDesmarca = '';
                                                            $onclickDesmarca = '';
                                                        }

									//ordem id = id_produto - id_disciplina - id_contratante/contratada - id_fase
							echo '<input class="'.$classDesmarca.'" onclick="'.$onclickDesmarca.'" type="checkbox" name="fdpid[]" id="fdpid" value="'.$dados[$i]['codigo'].'" '.$checadoSim.' '.$verificaTipo.'>&nbsp;'.$dados[$i]['descricao'].'<br>';
									
									//habilita botao salvar
							if(!$verificaTipo) $btnSalvar = true;
									
							$i++;
						}
						
						echo '</div>';
						
					} else {
						echo '<div style="text-align:center;color:red;">Nenhum registro encontrado</div>';
					}					
					
					?>
					</td>
<!-- 				<td width="25%" style="text-align: center;"><B>ELABORAÇÃO</B></td>
					<td width="25%" style="text-align: center;"><B>CONSTRUÇÃO</B></td>
					<td width="25%" style="text-align: center;"><B>TRANSIÇÃO</B></td> -->
				</tr>
				
				<tr>
					<td style="text-align: center;" bgcolor="#dcdcdc"><B>ELABORAÇÃO</B></td>
					<td>
					<?
					$fase = 2;
					//verifica se esta checado com o contratante ou contratada
					if($tpeid == '1') $tpeidAux = '2';
					else $tpeidAux = '1';
					
					$verificaTipo = "";
					$sql = "SELECT count(sp.fdpid) as total 
							FROM fabrica.servicofaseproduto sp 
							INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
							INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
							INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
							WHERE sp.ansid = {$ansid} 
							and fdp.fdpstatus = 'A'
							and fd.fasid = {$fase}
							and fd.dspid = {$dspid}
							and sp.tpeid = {$tpeidAux}";
					$total = $db->pegaUm($sql);
					if($total > 0) $verificaTipo = "disabled";		
					
					$sql = "SELECT
										f.fdpid AS codigo,
										p.prddsc AS descricao
									FROM
										fabrica.fasedisciplinaproduto f
									INNER JOIN
										fabrica.fasedisciplina fd ON fd.fsdid = f.fsdid
									INNER JOIN
										 fabrica.produto p ON p.prdid = f.prdid
									WHERE
										p.prdstatus='A' 
										and f.fdpstatus = 'A'
										AND fd.fasid = {$fase}
										AND fd.dspid = ".$dspid."
										order by 2";
							//dbg($sql,1);
					$dados = $db->carregar($sql);
					$i=0;
					if($dados){
						
						echo '<input type="checkbox" class="marcatodos" name="todos_'.$fase.'" id="todos_'.$fase.'" '.$verificaTipo.' value="" onclick="marcaTodos('.$fase.',this)">&nbsp;Todos<br>';
						echo '<div id="divMarcaTodos_'.$fase.'">';
						
						foreach($dados as $dado){
									
									//verifica se esta checado
							$checadoSim = "";
							$sql = "SELECT sp.fdpid 
											FROM fabrica.servicofaseproduto sp 
											INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
											INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
											INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
											WHERE sp.ansid = {$ansid} 
											and fdp.fdpstatus = 'A'
											and fd.fasid = {$fase}
											and fd.dspid = {$dspid}
											and sp.tpeid = {$tpeid}
											and sp.fdpid = {$dado['codigo']}";
							$checado = $db->pegaUm($sql);
							if($checado) $checadoSim = "checked";


                                                        if($dados[$i]['descricao'] == 'Serviço sem necessidade de artefato'){
                                                            $classDesmarca = 'desmarcar';
                                                            $onclickDesmarca = "desmarcaTodos('$fase',this)";
                                                        }else{
                                                            $classDesmarca = '';
                                                            $onclickDesmarca = '';
                                                        }

									//ordem id = id_produto - id_disciplina - id_contratante/contratada - id_fase
							echo '<input class="'.$classDesmarca.'" onclick="'.$onclickDesmarca.'" type="checkbox" name="fdpid[]" id="fdpid" value="'.$dados[$i]['codigo'].'" '.$checadoSim.' '.$verificaTipo.'>&nbsp;'.$dados[$i]['descricao'].'<br>';
									
									//habilita botao salvar
							if(!$verificaTipo) $btnSalvar = true;
									
							$i++;
						}
						
						echo '</div>';
						
					} else {
						echo '<div style="text-align:center;color:red;">Nenhum registro encontrado</div>';
					}					
					
					?>
					</td>
<!-- 				<td width="25%" style="text-align: center;"><B>ELABORAÇÃO</B></td>
					<td width="25%" style="text-align: center;"><B>CONSTRUÇÃO</B></td>
					<td width="25%" style="text-align: center;"><B>TRANSIÇÃO</B></td> -->
				</tr>
				
				<tr>
					<td style="text-align: center;" bgcolor="#dcdcdc"><B>CONSTRUÇÃO</B></td>
					<td>
					<?
					$fase = 3;
					//verifica se esta checado com o contratante ou contratada
					if($tpeid == '1') $tpeidAux = '2';
					else $tpeidAux = '1';
					
					$verificaTipo = "";
					$sql = "SELECT count(sp.fdpid) as total 
							FROM fabrica.servicofaseproduto sp 
							INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
							INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
							INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
							WHERE sp.ansid = {$ansid} 
							and fdp.fdpstatus = 'A'
							and fd.fasid = {$fase}
							and fd.dspid = {$dspid}
							and sp.tpeid = {$tpeidAux}";
					$total = $db->pegaUm($sql);
					if($total > 0) $verificaTipo = "disabled";		
					
					$sql = "SELECT
										f.fdpid AS codigo,
										p.prddsc AS descricao
									FROM
										fabrica.fasedisciplinaproduto f
									INNER JOIN
										fabrica.fasedisciplina fd ON fd.fsdid = f.fsdid
									INNER JOIN
										 fabrica.produto p ON p.prdid = f.prdid
									WHERE
										p.prdstatus='A' 
										and f.fdpstatus = 'A'
										AND fd.fasid = {$fase}
										AND fd.dspid = ".$dspid."
										order by 2";
							//dbg($sql,1);
					$dados = $db->carregar($sql);
					$i=0;
					if($dados){
						
						echo '<input type="checkbox" class="marcatodos" name="todos_'.$fase.'" id="todos_'.$fase.'" '.$verificaTipo.' value="" onclick="marcaTodos('.$fase.',this)">&nbsp;Todos<br>';
						echo '<div id="divMarcaTodos_'.$fase.'">';
						
						foreach($dados as $dado){
									
									//verifica se esta checado
							$checadoSim = "";
							$sql = "SELECT sp.fdpid 
											FROM fabrica.servicofaseproduto sp 
											INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
											INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
											INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
											WHERE sp.ansid = {$ansid} 
											and fdp.fdpstatus = 'A'
											and fd.fasid = {$fase}
											and fd.dspid = {$dspid}
											and sp.tpeid = {$tpeid}
											and sp.fdpid = {$dado['codigo']}";
							$checado = $db->pegaUm($sql);
							
							if($checado) $checadoSim = "checked";

                                                        if($dados[$i]['descricao'] == 'Serviço sem necessidade de artefato'){
                                                            $classDesmarca = 'desmarcar';
                                                            $onclickDesmarca = "desmarcaTodos('$fase',this)";
                                                        }else{
                                                            $classDesmarca = '';
                                                            $onclickDesmarca = '';
                                                        }

									//ordem id = id_produto - id_disciplina - id_contratante/contratada - id_fase
							echo '<input class="'.$classDesmarca.'" onclick="'.$onclickDesmarca.'" type="checkbox" name="fdpid[]" id="fdpid" value="'.$dados[$i]['codigo'].'" '.$checadoSim.' '.$verificaTipo.'>&nbsp;'.$dados[$i]['descricao'].'<br>';
									
									//habilita botao salvar
							if(!$verificaTipo) $btnSalvar = true;
									
							$i++;
						}
						
						echo '</div>';
						
					} else {
						echo '<div style="text-align:center;color:red;">Nenhum registro encontrado</div>';
					}					
					
					?>
					</td>
<!-- 				<td width="25%" style="text-align: center;"><B>ELABORAÇÃO</B></td>
					<td width="25%" style="text-align: center;"><B>CONSTRUÇÃO</B></td>
					<td width="25%" style="text-align: center;"><B>TRANSIÇÃO</B></td> -->
				</tr>
				
				<tr>
					<td style="text-align: center;" bgcolor="#dcdcdc"><B>TRANSIÇÃO</B></td>
					<td>
					<?
					$fase = 4;
					//verifica se esta checado com o contratante ou contratada
					if($tpeid == '1') $tpeidAux = '2';
					else $tpeidAux = '1';
					
					$verificaTipo = "";
					$sql = "SELECT count(sp.fdpid) as total 
							FROM fabrica.servicofaseproduto sp 
							INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
							INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
							INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
							WHERE sp.ansid = {$ansid} 
							and fdp.fdpstatus = 'A'
							and fd.fasid = {$fase}
							and fd.dspid = {$dspid}
							and sp.tpeid = {$tpeidAux}";
					$total = $db->pegaUm($sql);
					if($total > 0) $verificaTipo = "disabled";		
					
					$sql = "SELECT
										f.fdpid AS codigo,
										p.prddsc AS descricao
									FROM
										fabrica.fasedisciplinaproduto f
									INNER JOIN
										fabrica.fasedisciplina fd ON fd.fsdid = f.fsdid
									INNER JOIN
										 fabrica.produto p ON p.prdid = f.prdid
									WHERE
										p.prdstatus='A' 
										and f.fdpstatus = 'A'
										AND fd.fasid = {$fase}
										AND fd.dspid = ".$dspid."
										order by 2";
							//dbg($sql,1);
					$dados = $db->carregar($sql);
					$i=0;
					if($dados){
						
						echo '<input type="checkbox" class="marcatodos" name="todos_'.$fase.'" id="todos_'.$fase.'" '.$verificaTipo.' value="" onclick="marcaTodos('.$fase.',this)">&nbsp;Todos<br>';
						echo '<div id="divMarcaTodos_'.$fase.'">';
						
						foreach($dados as $dado){
									
									//verifica se esta checado
							$checadoSim = "";
							$sql = "SELECT sp.fdpid 
											FROM fabrica.servicofaseproduto sp 
											INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
											INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
											INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
											WHERE sp.ansid = {$ansid} 
											and fdp.fdpstatus = 'A'
											and fd.fasid = {$fase}
											and fd.dspid = {$dspid}
											and sp.tpeid = {$tpeid}
											and sp.fdpid = {$dado['codigo']}";
							$checado = $db->pegaUm($sql);
							if($checado) $checadoSim = "checked";

                                                        if($dados[$i]['descricao'] == 'Serviço sem necessidade de artefato'){
                                                            $classDesmarca = 'desmarcar';
                                                            $onclickDesmarca = "desmarcaTodos('$fase',this)";
                                                        }else{
                                                            $classDesmarca = '';
                                                            $onclickDesmarca = '';
                                                        }

									//ordem id = id_produto - id_disciplina - id_contratante/contratada - id_fase
							echo '<input class="'.$classDesmarca.'" onclick="'.$onclickDesmarca.'" type="checkbox" name="fdpid[]" id="fdpid" value="'.$dados[$i]['codigo'].'" '.$checadoSim.' '.$verificaTipo.'>&nbsp;'.$dados[$i]['descricao'].'<br>';
									
									//habilita botao salvar
							if(!$verificaTipo) $btnSalvar = true;
									
							$i++;
						}
						
						echo '</div>';
						
					} else {
						echo '<div style="text-align:center;color:red;">Nenhum registro encontrado</div>';
					}					
					
					?>
					</td>
<!-- 				<td width="25%" style="text-align: center;"><B>ELABORAÇÃO</B></td>
					<td width="25%" style="text-align: center;"><B>CONSTRUÇÃO</B></td>
					<td width="25%" style="text-align: center;"><B>TRANSIÇÃO</B></td> -->
				</tr>
				

			</table>
		
			<div style="text-align:center;width:100%;background-color:#c0c0c0;padding-top:5px;padding-bottom:5px;">
				<?if($btnSalvar){?>
					<input type="button" value="Salvar" onclick="validaForm();">
				<?}else{?>
					<input type="button" value="Fechar" onclick="self.close();">
				<?}?>
			</div>
		</form>
	</body>
</html>

<script type="text/javascript">


function validaForm(){

	
	document.formPopLista.varaux.value = '1';
	document.formPopLista.submit();
	
}


function marcaTodos(fase,idcampo){

	var obj = document.getElementById('divMarcaTodos_'+fase).getElementsByTagName('input');

	for(i=0;i<obj.length;i++){
            if(obj[i].className != 'desmarcar')
                obj[i].checked = idcampo.checked;
            else
                obj[i].checked = '';
	}
		
}

function desmarcaTodos(fase,idcampo){

    	var obj = document.getElementById('divMarcaTodos_'+fase).getElementsByTagName('input');

	for(i=0;i<obj.length;i++){

            if(obj[i].className != 'desmarcar')
                obj[i].checked = '';
            
            document.getElementById('todos_'+fase).checked = '';
            
	}

}

$('input[type=checkbox]').not('.desmarcar, .marcatodos').click(function(){

    var fase = $(this).parent().attr('id').split('_');
    fase = fase[1];

    $('#divMarcaTodos_' + fase + ' .desmarcar, #todos_' + fase).attr('checked', '');

});

</script>