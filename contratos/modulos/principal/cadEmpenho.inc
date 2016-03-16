<?php 
function listaSolicitacoes( $dados ){
	
	global $db;
	
	if($dados['unicod']){
		$busca = $dados['unicod'];
		$campo = 'unidsc,';
		$campoWh = 'unicod';
		$inner = 'INNER JOIN public.unidade ung ON ung.unicod = emu.unicod';
	}else{
		$campo = 'ungdsc,';
		$campoWh = 'ungcod';
		$inner = 'INNER JOIN public.unidadegestora ung ON ung.ungcod = emu.ungcod';
		$busca = $dados['ungcod'];
	}
	
	$sql = "SELECT 
				'<div align=\"center\">
					<img class=\"alterar\" border=\"0\" title=\"Alterar empenho.\" src=\"../imagens/alterar.gif\"  id=\"'||emuid||'\" />
				</div>'  as acao,
				$campo
				'<a title=\"Alterar empenho.\" href=\"evento.php?modulo=principal/cadEmpenho&acao=A&emuid='||emuid||'\" style=\"color:blue\">'||empcod||' - '||empdsc||'</a>' as empenho
			FROM 
				evento.empenho_unidadegestora emu
			$inner
			WHERE
				emu.$campoWh = '".$busca."'
			ORDER BY
				empenho ";
	$arCabecalho = array("&nbsp","Unidade Gestora", "Empenho"); 
	unset($_REQUEST['req']);
	unset($_POST['req']);
	
	$db->monta_lista( $sql, $arCabecalho, 20, 10, 'N', '');
	
}

function alterarEmpenho($dados){
	
	global $db;
	
//	$sql = "SELECT
//				emuid
//			FROM evento.empenho_unidadegestora
//			WHERE
//				ungcod = ".$dados['ungcod'].", 
//			    AND empcod = '".$dados['empcod']."'";
//	$emuid = $db->pegaUm($sql);
//	
//	if($emuid){
//		echo "<script>
//				alert('Empenho já cadastrado.');
//				window.location = 'evento.php?modulo=principal/cadEmpenho&acao=A&emuid='.$emuid;
//			  </script>";
//	}
	
	if($dados['emuid'] && $dados['ungcod'] && $dados['empcod'] && is_array($dados['resp'])){
		
		if(strlen($dados['ungcod'])==5){
			$campo = 'unicod';
		}else{
			$campo = 'ungcod';
		}
		
		$sql = "UPDATE evento.empenho_unidadegestora SET
			    	$campo = ".$dados['ungcod'].", 
			    	empdsc = ".$dados['empdsc'].", 
			    	empcod = '".$dados['empcod']."'
			    WHERE
			    	emuid = ".$dados['emuid'].";";

		$sql = "UPDATE evento.empenho_responssavel SET erpstatus = 'I' WHERE emuid = ".$dados['emuid'].";";
		foreach($dados['resp'] as $responssavel){
			
			$sql .= "INSERT INTO evento.empenho_responssavel(
				            emuid, usucpf)
				     VALUES (".$dados['emuid'].", '".$responssavel."');";
			
		}
		
		if($db->executar($sql)){
			$db->commit();
			$db->sucesso('principal/cadEmpenho','&emuid='.$dados['emuid']);
		}
		
	}else{
		
		echo "<script>
				alert('Dados insuficientes para alterar esse empenho.');
				window.location = 'evento.php?modulo=principal/cadEmpenho&acao=A';
			  </script>";
		
	}
	
}

function cadastrarEmpenho($dados){
	
	global $db;
	
	$sql = "SELECT
				emuid
			FROM evento.empenho_unidadegestora
			WHERE
				ungcod = '".$dados['ungcod']."' 
			    AND empcod = '".$dados['empcod']."'
			LIMIT 1";
	$emuid = $db->pegaUm($sql);
	if($emuid){
		echo "<script>
				alert('Empenho já cadastrado.');
				window.location = 'evento.php?modulo=principal/cadEmpenho&acao=A&emuid=".$emuid."';
			  </script>";
	}else{
		if($dados['ungcod'] && $dados['empcod'] && is_array($dados['resp'])){
			
			if(strlen($dados['ungcod'])==5){
				$campo = 'unicod';
			}else{
				$campo = 'ungcod';
			}
			
			$sql = "INSERT INTO evento.empenho_unidadegestora(
				            $campo, empcod, empdsc)
				    VALUES (".$dados['ungcod'].", '".$dados['empcod']."', '".$dados['empdsc']."')
				    RETURNING emuid;";
			
			$emuid = $db->pegaUm($sql);
			
			$sql = '';

			foreach($dados['resp'] as $responssavel){
				
				$sql .= "INSERT INTO evento.empenho_responssavel(
					            emuid, usucpf)
					     VALUES (".$emuid.", '".$responssavel."');";
				
			}
			
			if($db->executar($sql)){
				$db->commit();
				$db->sucesso('principal/cadEmpenho','&emuid='.$emuid);
			}
			
		}else{
			
			echo "<script>
					alert('Dados insuficientes para cadastrar esse empenho.');
					window.location = 'evento.php?modulo=principal/cadEmpenho&acao=A';
				  </script>";
			
		}
	}
	
}


function montacomboResnpossaveis($dados){
	
	global $db;

	if(strlen($dados['ungcod'])==5){
		$campoWh = 'unicod';
	}else{
		$campoWh = 'ungcod';
	}
	
	$sql = "SELECT 
				usu.usucpf as codigo, 
				usu.usucpf||' - '||usunome as descricao
			FROM 
				seguranca.usuario usu
			INNER JOIN evento.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			WHERE
				rpu.$campoWh = '".$dados['ungcod']."'";

	combo_popup('resp', $sql, 'Responssáveis', '400x400', 0, array(), '', 'S', false, false, 5, 400 );
}

if($_REQUEST['req']){
	$_REQUEST['req']($_REQUEST);
	die();
}

$notUngcod = Array();
$notUnicod = Array();
if(!$db->testa_superuser()){
	
	$perfils = Array(EVENTO_PERFIL_SOLICITADOR, EVENTO_PERFIL_ADMINISTRADOR_DIARIAS);
	
	if( possuiPerfil( $perfils ) ){
		$sql = "SELECT DISTINCT 
					p.ungcod 
				FROM 
					evento.usuarioresponsabilidade ur
				INNER JOIN public.unidadegestora p ON ur.ungcod = p.ungcod
				WHERE
					ur.rpustatus = 'A' and
					ur.usucpf = '".$_SESSION['usucpf']."' and
					ur.pflcod in ( ".implode(',',$perfils)." ) and
					ur.prsano = '".$_SESSION['exercicio']."'";
		$notUngcod = $db->carregarColuna($sql);
		$sql = "SELECT DISTINCT 
					p.unicod as codigo, 
					p.unicod || ' - ' || p.unidsc as descricao
				FROM 
					evento.usuarioresponsabilidade ur
				INNER JOIN public.unidade p ON ur.unicod = p.unicod
				WHERE
					ur.rpustatus = 'A' and
					ur.usucpf = '".$_SESSION['usucpf']."' and
					ur.pflcod in ( ".implode(',',$perfils)." ) and
					ur.prsano = '".$_SESSION['exercicio']."'";
		$notUnicod = $db->carregarColuna($sql);
	}else{
		echo "<script>
				alert('Acesso negado.');
				window.history.back(-1); 
			  </script>";
	}
}

if($_REQUEST['emuid']){
	
	$sql = "SELECT 
				ung.ungcod, 
				ung.ungdsc,
				empcod,
				empdsc
			FROM 
				evento.empenho_unidadegestora eun
			INNER JOIN public.unidadegestora ung ON ung.ungcod = eun.ungcod
			WHERE
				emuid = ".$_REQUEST['emuid'];
	
	$emp = $db->pegaLinha($sql);

//	if(!$emp['ungcod']){
//		echo "<script>
//				alert('Empenho não encontrado.');
//				window.location = 'evento.php?modulo=principal/cadEmpenho&acao=A';
//			  </script>";
//	}
	
	if($emp == ''){
		$sql = "SELECT 
					uni.unicod, 
					uni.unidsc,
					empcod,
					empdsc
				FROM 
					evento.empenho_unidadegestora euo
				INNER JOIN public.unidade uni ON uni.unicod = euo.unicod
				WHERE
					emuid = ".$_REQUEST['emuid'];
		
		$emp = $db->pegaLinha($sql);

		if(!$emp['unicod']){
			echo "<script>
					alert('Empenho não encontrado.');
					window.location = 'evento.php?modulo=principal/cadEmpenho&acao=A';
				  </script>";
		}
	}
	
	$sql = "SELECT 
				usu.usucpf as codigo, 
				usu.usucpf||' - '||usu.usunome as descricao
  			FROM 
  				evento.empenho_responssavel erp
  			INNER JOIN seguranca.usuario usu ON usu.usucpf = erp.usucpf
  			WHERE
  				erpstatus = 'A'
  				AND emuid = ".$_REQUEST['emuid'];

	$resp = $db->carregar($sql);
}


include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

if($_REQUEST['emuid']){
	$abas = Array(Array('descricao'=>'Cadastrar Empenho',
				  		'link'     =>'evento.php?modulo=principal/cadEmpenho&acao=A'),
				  Array('descricao'=>'Alterar Empenho',
			  			'link'     =>'evento.php?modulo=principal/cadEmpenho&acao=A&emuid='.$_REQUEST['emuid'])
					);
	$url = 'evento.php?modulo=principal/cadEmpenho&acao=A&emuid='.$_REQUEST['emuid'];
}else{
	$abas = Array(Array('descricao'=>'Cadastrar Empenho',
			  		'link'     =>'evento.php?modulo=principal/cadEmpenho&acao=A')
				);
	$url = 'evento.php?modulo=principal/cadEmpenho&acao=A';
}

echo montarAbasArray($abas, $url);

$subtitulo = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Campo obrigatório.';
monta_titulo( 'Cadastro de Empenho ', $subtitulo );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function() {


	jQuery('#voltar').click(function(){

		window.history.back(-1);
	});

	jQuery('#novo').click(function(){
		window.location = 'evento.php?modulo=principal/cadEmpenho&acao=A';
	});

	jQuery('.alterar').live('click',function(){
		var emuid = jQuery(this).attr('id')
		window.location = 'evento.php?modulo=principal/cadEmpenho&acao=A&emuid='+emuid;
	});
	

	function validarPendencias(){
		var pendencia = false;
		if(jQuery('#ungcod').val()==''){alert('Campo \'Unidade Gestora\' obrigatório.');jQuery('#ungcod').focus();jQuery('#inserir').attr('disabled',false);return false; }
		if(jQuery.trim(jQuery('#empcod').val())==''){alert('Campo \'Código do Empenho\' obrigatório.');jQuery('#empcod').focus();jQuery('#inserir').attr('disabled',false);return false; }
		if(jQuery.trim(jQuery('#empdsc').val())==''){alert('Campo \'Descrição do Empenho\' obrigatório.');jQuery('#empdsc').focus();jQuery('#inserir').attr('disabled',false);return false; }
		jQuery('#resp option').each(function (){
			if(jQuery(this).val()==''){ pendencia = true; }
		});
		if(pendencia){alert('Campo \'Responssáveis\' obrigatório.');jQuery(this).attr('disabled',false);return false; }	

		return true;
	}

	function submete(tipo){
		var req = '';
		if(tipo=='I'){req='cadastrarEmpenho'
		}else if(tipo=='A'){req='alterarEmpenho'
		}else{return false;}
		jQuery('#req').val(req);
		jQuery('#formCadEmp').submit();
	}
	
	jQuery('#inserir').click(function(){

		var pendencia = true;

		jQuery(this).attr('disabled',true);
		
		jQuery('#resp option').each(function (){
			jQuery(this).attr('selected',true);
		});
		
		if(validarPendencias()){
			submete('I');
		}
	});


	jQuery('#alterar').click(function(){

		var pendencia = true;

		jQuery(this).attr('disabled',true);
		
		jQuery('#resp option').each(function (){
			jQuery(this).attr('selected',true);
		});
		
		if(validarPendencias()){
			submete('A');
		}
	});
	
	jQuery('#ungcod').change(function(){

		var ungcod = jQuery(this).val();

		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "req=montacomboResnpossaveis&ungcod="+ungcod,
			async: false,
			success: function(msg){
				jQuery('#tdResp').html(msg);
				jQuery.ajax({
					type: "POST",
					url: window.location,
					data: "req=listaSolicitacoes&ungcod="+ungcod,
					async: false,
					success: function(msg){
						jQuery('#lista').html(msg);
					}
				});
			}
		});
	});

});

</script>
<form method="post" name="formCadEmp" id="formCadEmp" action="">
	<input type="hidden" name="req"   id="req"   value=""/>
	<input type="hidden" name="emuid" id="emuid" value="<?=$_REQUEST['emuid'] ?>"/>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita">Unidade Gestora/Orçamentária</td>
			<td>
			<?php 
//				// Unidades Gestoras
				$sql = "SELECT 
							ungcod as codigo, 
							ungdsc as descricao
						FROM 
							public.unidadegestora
						WHERE
							ungcod NOT IN ('152004','152005') AND
							".(count($notUngcod)>0 ? "ungcod IN ('".implode('\',\'', $notUngcod)."') AND" : "")."
							ungstatus = 'A' 
						GROUP BY 
							ungcod, 
							ungdsc
						ORDER BY 
							ungdsc;";
//				
//				$ungcod = $emp['ungcod'];
//				$db->monta_combo('ungcod',$sql,'S','Selecione...','',$emp['ungcod'],'','','S', 'ungcod');
			?>
				<select style="width: 200px;" class="CampoEstilo" name="ungcod"  id="ungcod">
					<option value="" />Selecione... </option>
					<optgroup label="Unidade Gestora">
						<?php
							$sql = "SELECT 
										ungcod as codigo, 
										ungdsc as descricao
									FROM 
										public.unidadegestora
									WHERE
										ungcod NOT IN ('152004','152005') AND
										".(count($notUngcod)>0 ? "ungcod IN ('".implode('\',\'', $notUngcod)."') AND" : "")."
										ungstatus = 'A' 
									GROUP BY 
										ungcod, 
										ungdsc
									ORDER BY 
										ungdsc;";
						?>
						<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
						<option value="<?= $tipo['codigo'] ?>"<?=$emp['ungcod'] == $tipo['codigo'] ? 'selectec="selected"' : ''; ?>><?= $tipo['descricao'] ?></option>
						<?php endforeach; ?>
					</optgroup>
					<optgroup label="Unidade Orçamentária">
						<?php 
							 $sql = "SELECT 
										unicod as codigo, 
										unidsc as descricao
									FROM 
										public.unidade
									WHERE
										unistatus = 'A' AND
										unitpocod = 'U' AND
										".(count($notUnicod)>0 ? "unicod IN ('".implode('\',\'', $notUnicod)."') AND" : "")."
										orgcod = '26000' AND
										unicod != '26101' AND
										unicod ilike '26%'
									GROUP BY 
										unicod, 
										unidsc
									ORDER BY 
										unicod;";
						?>
						<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
						<option value="<?= $tipo['codigo'] ?>"<?=$emp['unicod'] == $tipo['codigo'] ? 'selected="selected"' : ''; ?> ><?= $tipo['descricao'] ?></option>
						<?php endforeach; ?>
					</optgroup>
				</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Código do Empenho</td>
			<td>
			<?php 
				$empcod = $emp['empcod']; 
				echo campo_texto( 'empcod', 'S', 'S', '', 15, 12, '', '','','','','id="empcod"' ); 
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descricao</td>
			<td>
			<?php 
				$empdsc = $emp['empdsc']; 
				echo campo_texto( 'empdsc', 'S', 'S', '', 63, 100, '', '','','','','id="empdsc"' ); 
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Responsáveis</td>
			<td id="tdResp">
			<?php
				if($_REQUEST['emuid']){
					
					$sql = "SELECT 
								usu.usucpf as codigo, 
								usu.usucpf||' - '||usunome as descricao
							FROM 
								seguranca.usuario usu
							INNER JOIN evento.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
							WHERE
								rpu.ungcod = '".$emp['ungcod']."'";
					
					if(is_array($resp)){
						combo_popup('resp', $sql, 'Responssáveis', '400x400', 0, array(), '', 'S', false, false, 5, 400, '', '', '', '', $resp );
					}else{
						combo_popup('resp', $sql, 'Responssáveis', '400x400', 0, array(), '', 'S', false, false, 5, 400 );
					}
				}else{
					$sql = "SELECT	1 as codigo, 1 as descricao";
					combo_popup('resp', $sql, 'Responssáveis', '400x400', 0, array(), '', 'N', false, false, 5, 400, '', '', '', '', '' );
				}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2" >
				<div style="float:left">
					<input type="button" id="voltar" value="Voltar" />	
				</div>
				<?php if($_REQUEST['emuid']){?>
				<input type="button" id="alterar" value="Salvar Alterações" />
				<div style="float:right">
					<input type="button" id="novo" value="Cadastrar novo empenho" />	
				</div>
				<?php }else{?>
				<input type="button" id="inserir" value="Cadastrar Empenho" />
				<?php }?>	
			</td>
		</tr>
	</table>
</form>
<div id="lista">
	<?php 
	if($_REQUEST['emuid']){
		listaSolicitacoes( $emp );
	}
	
	?>
</div>