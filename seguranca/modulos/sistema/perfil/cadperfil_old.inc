<?
 /* 
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Fernando Bagno <fernandobagno@gmail.com>
   Módulo:cadperfil.inc
   Finalidade: permitir o controle de cadastro dos perfis
   */

if(!$_REQUEST["modid"]) $_REQUEST["modid"] = "NULL";

switch( $_REQUEST["requisicao"] ){
	
	case "lista":
		
		if ( $_REQUEST["sisid"] ){
			$sql = "SELECT
					'<center>
						<img src=\"/imagens/alterar.gif\" onclick=\"alterarPerfil( ' || pflcod || ' );\" style=\"cursor:pointer;\"/>
						<img src=\"/imagens/excluir.gif\" onclick=\"excluirPerfil( ' || pflcod || ' );\" style=\"cursor:pointer;\"/>
					</center>',
					pflnivel,
					pfldsc,
					pflcod,
					CASE WHEN 
						pflsuperuser = false
					THEN
						'<center><img src=\"/imagens/exclui_p.gif\"/></center>'
					ELSE
						'<center><img src=\"/imagens/check_p.gif\"/></center>'
					END as superusuario,
					CASE WHEN 
						pflinddelegar = false
					THEN
						'<center><img src=\"/imagens/exclui_p.gif\"/></center>'
					ELSE
						'<center><img src=\"/imagens/check_p.gif\"/></center>'
					END as delegar,
					CASE WHEN 
						pflsuporte = false
					THEN
						'<center><img src=\"/imagens/exclui_p.gif\"/></center>'
					ELSE
						'<center><img src=\"/imagens/check_p.gif\"/></center>'
					END as suporte,
					CASE WHEN 
						(pflsncumulativo = false OR pflsncumulativo is null)
					THEN
						'<center><img src=\"/imagens/exclui_p.gif\"/></center>'
					ELSE
						'<center><img src=\"/imagens/check_p.gif\"/></center>'
					END as cumulativo
				FROM
					seguranca.perfil per
				WHERE
					sisid = {$_REQUEST["sisid"]} AND pflstatus = 'A'
				ORDER BY
					pflnivel, pfldsc";
			$arrDados = $db->carregar($sql);
			
			if($arrDados){
				foreach($arrDados as $k => $dado){
					$sql = "select 
								pfldsc 
							from 
								seguranca.perfilpermissao pfp
							inner join
								seguranca.perfil per ON per.pflcod = pfp.pflcodfilho
							where 
								pflcodpai = {$dado['pflcod']}";
					$arrPflcodfilho = $db->carregarColuna($sql);
					$arrDados[$k]['pflcodfilho'] = ($arrPflcodfilho ? implode(", ",$arrPflcodfilho) : "N/A");
					unset($arrDados[$k]['pflcod']);
				}
			}
			
			$cabecalho = array( "Ação", "Nível", "Perfil", "Super Usuário", "Pode Delegar", "Suporte", "Cumulativo" ,"Perfil Associado (Filho)");
			$db->monta_lista_array( $arrDados, $cabecalho, 20, 4, 'N','center', '', '', '', '' );
			
		}
			
		die;
		
	break;
	
	case "modulos":
		
		if ( $_REQUEST["sisid"] ){
			$sql = "SELECT DISTINCT 
								modid AS codigo, 
								moddsc AS descricao 
							FROM 
								seguranca.modulo 
							where 
								modstatus='A'
								and sisid = ".$_REQUEST["sisid"]."
							ORDER BY 
								descricao";
			
			$db->monta_combo( "modid", $sql, 'S', "Selecione o Módulo", "", "", "", "", "N", "modid" );
		}
			
		die;
		
	break;
	
	case "busca":
		
		$sql = "SELECT
					pflcod,
					pfldsc,
					pflfinalidade,
					pflnivel,
					pflresponsabilidade,
					pflsuperuser,
					pflsuporte,
					pflinddelegar,
					pflsncumulativo,
					modid
				FROM
					seguranca.perfil per 
				WHERE
					pflcod = {$_REQUEST["pflcod"]}";
		
		$dados = $db->pegaLinha( $sql );
		
		if($_REQUEST['pflcod']){
			$sqlFilhos = "SELECT DISTINCT 
						pflcod AS codigo, 
						pfldsc AS descricao 
					FROM
						seguranca.perfil
					WHERE
						sisid = {$_SESSION["sisid"]} AND pflstatus = 'A'
					AND
						pflcod in ( select pflcodfilho from seguranca.perfilpermissao where pflcodpai = '{$_REQUEST['pflcod']}')
					ORDER BY
						pfldsc";
			$pflcodfilho = $db->carregar($sqlFilhos);
			if($pflcodfilho){
				foreach($pflcodfilho as $filho){
					$dados["pflcodfilho"][] = array("codigo" => $filho['codigo'], "descricao" => iconv("ISO-8859-1", "UTF-8", $filho["descricao"]));	
				}
			}else{
				$dados["pflcodfilho"] = array();
			}
		}
		
		$dados["pfldsc"] = iconv("ISO-8859-1", "UTF-8", $dados["pfldsc"]);
		$dados["pflfinalidade"] = iconv("ISO-8859-1", "UTF-8", $dados["pflfinalidade"]);
		
		echo simec_json_encode($dados);
		die;
		
	break;
	
	case "inserir":
		
		$existePerfil = $db->pegaUm( "SELECT pfldsc FROM seguranca.perfil 
										WHERE pfldsc = '{$_REQUEST["pfldsc"]}' AND sisid = {$_REQUEST["sisid"]} AND pflstatus = 'A'" );

		if ( $existePerfil ){
			
			print "<script>
					   alert('Este perfil já está cadastrado!');
					   history.back(-1);
				   </script>";
			
		}else{
			
			$pflcod = $db->pegaUm( "INSERT INTO seguranca.perfil( sisid, 
														  pfldsc, 
														  pflfinalidade, 
														  pflnivel,
														  pflresponsabilidade, 
														  pflsuperuser, 
														  pflsuporte, 
														  pflinddelegar, 
														  pflsncumulativo,
														  modid,
														  pflstatus )
												 VALUES( {$_REQUEST["sisid"]},
												 		 '{$_REQUEST["pfldsc"]}',
												 		 '{$_REQUEST["pflfinalidade"]}',
												 		 '{$_REQUEST["pflnivel"]}',
												 		 '{$_REQUEST["pflresponsabilidade"]}',
												 		 '{$_REQUEST["pflsuperuser"]}',
												 		 '{$_REQUEST["pflsuporte"]}',
												 		 '{$_REQUEST["pflinddelegar"]}',
												 		 '{$_REQUEST["pflsncumulativo"]}',
												 		 {$_REQUEST["modid"]},
												 		 'A' ) returning pflcod" );
			
			if($pflcod){
				$db->executar( "delete from seguranca.perfilpermissao where pflcodpai = $pflcod" );
			}
			if($_REQUEST["pflcodfilho"][0] && $pflcod){
				foreach($_REQUEST["pflcodfilho"] as $pflcodfilho){
					if($pflcodfilho != $pflcod){
						$sqlFilho.= " insert into 
								seguranca.perfilpermissao 
							(pflcodfilho,pflcodpai) 
								values 
							('$pflcodfilho','$pflcod');";
					}
				}
				
				$db->executar($sqlFilho);
			}
			$db->commit();
			$db->sucesso( "sistema/perfil/cadperfil" );
			
		}
		
	break;
	
	case "alterar":
		
		$db->executar( "UPDATE 
							seguranca.perfil 
						SET
							pfldsc 				= '{$_REQUEST["pfldsc"]}',
							pflfinalidade   	= '{$_REQUEST["pflfinalidade"]}',
							pflnivel 	    	= {$_REQUEST["pflnivel"]},
							pflresponsabilidade = '{$_REQUEST["pflresponsabilidade"]}',
							pflsuperuser    	= '{$_REQUEST["pflsuperuser"]}',
							pflsuporte 	    	= '{$_REQUEST["pflsuporte"]}',
							pflinddelegar   	= '{$_REQUEST["pflinddelegar"]}',
							modid			   	= {$_REQUEST["modid"]},
							pflsncumulativo 	= '{$_REQUEST["pflsncumulativo"]}'
						WHERE
							pflcod = {$_REQUEST["pflcod"]}" );
		
			if($_REQUEST["pflcod"]){
				$db->executar( "delete from seguranca.perfilpermissao where pflcodpai = {$_REQUEST["pflcod"]}" );
			}

			if($_REQUEST["pflcodfilho"][0] && $_REQUEST["pflcod"]){
				foreach($_REQUEST["pflcodfilho"] as $pflcodfilho){
					if($pflcodfilho != $_REQUEST["pflcod"]){
						$sqlFilho.= " insert into 
								seguranca.perfilpermissao 
							(pflcodfilho,pflcodpai) 
								values 
							('$pflcodfilho','{$_REQUEST["pflcod"]}');";
					}
				}
				$db->executar($sqlFilho);
			}

		$db->commit();
		$db->sucesso( "sistema/perfil/cadperfil" );
		
	break;
	
	case "excluir":
		
		$db->executar( "UPDATE seguranca.perfil SET pflstatus = 'I' WHERE pflcod = {$_REQUEST["pflcod"]}" );
		$db->executar( "delete from seguranca.perfilpermissao where pflcodpai = {$_REQUEST["pflcod"]}" );
		$db->commit();
		$db->sucesso( "sistema/perfil/cadperfil" );
		
	break;
	
}

$caminhoAtual = $_SERVER["REQUEST_URI"];
$posicaoCaminho = strpos($caminhoAtual, 'acao');
$caminhoAtual = substr($caminhoAtual, 0 , $posicaoCaminho);

// Pega url para os js
$posicaoCaminhoJs = strpos($caminhoAtual, '?');
$caminhoAtualJs = substr($caminhoAtual, 0 , $posicaoCaminhoJs);

include APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Incluir Perfil" , '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	
	var caminhoAtualJs = '<?php echo $caminhoAtualJs; ?>';
	
	function listaPerfis( id ){
	
		var url = caminhoAtualJs + '?modulo=sistema/perfil/cadperfil&acao=I&requisicao=lista&sisid=' + id;
			
		var myAjax = new Ajax.Updater(
			"listaPerfis",
			url,
			{
				method: 'get',
				asynchronous: false
			});

		var url = caminhoAtualJs + '?modulo=sistema/perfil/cadperfil&acao=I&requisicao=modulos&sisid=' + id;
		
		var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			asynchronous: false,
			onComplete: function(resp) {
				$('tr_modulo').style.display = '';
				$('td_modulo').innerHTML = resp.responseText;
			}
		});
	}
	
	function validaInclusao(){
		
		var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
		var validacao = true;

		var formulario = document.getElementById('formulario');
		
		var sisid = document.formulario.sisid.value;
		if (sisid == ''){
			mensagem += 'Sistema \n';
			validacao = false;
		}
		
		var pfldsc = document.formulario.pfldsc.value;
		if (pfldsc == ''){
			mensagem += 'Descrição \n';
			validacao = false;
		}

		var pflnivel = document.formulario.pflnivel.value;
		if (pflnivel == ''){
			mensagem += 'Nível \n';
			validacao = false;
		}
		
		var pflinddelegar = document.formulario.pflinddelegar.value;
		if (pflinddelegar == ''){
			mensagem += 'Pode Delegar \n';
			validacao = false;
		}
		
		var pflsuperuser = document.formulario.pflsuperuser.value;
		if (pflsuperuser == ''){
			mensagem += 'Super Usuário \n';
			validacao = false;
		}
		
		var pflsuporte = document.formulario.pflsuporte.value;
		if (pflsuporte == ''){
			mensagem += 'Suporte \n';
			validacao = false;
		}
		
		var pflsncumulativo = document.formulario.pflsncumulativo.value;
		if (pflsncumulativo == ''){
			mensagem += 'Cumulativo \n';
			validacao = false;
		}
		
		if ( !validacao ){
			alert( mensagem );
			return false;
		}else{
			selectAllOptions(document.getElementById('pflcodfilho'));
			formulario.submit();
		}
			
	}
	
	function alterarPerfil( pflcod ){
	
		var i = 0;
		for(i = 0 ; i <= $('pflcodfilho').length ; i++){
			$('pflcodfilho').remove(i);
		}
	
		var url = caminhoAtualJs + '?modulo=sistema/perfil/cadperfil&acao=I&requisicao=busca';
		var parametros = "&pflcod=" + pflcod;
	
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(resp) {
					
					var json = resp.responseText.evalJSON();
					
					$('pflcod').value = json.pflcod;
					$('pfldsc').value = json.pfldsc;
					$('pflfinalidade').value = json.pflfinalidade;
					$('modid').value = json.modid;
					$('pflnivel').value = json.pflnivel;
					
					if(json.pflcodfilho.length > 0){
						var i = 0;
						for(i = 0 ; i <= json.pflcodfilho.length ; i++){
							$('pflcodfilho').insert('<option value="' + json.pflcodfilho[i]['codigo'] + '" selected="selected" >' + json.pflcodfilho[i]['descricao'] + '</option>');
						}
					}else{
						$('pflcodfilho').insert('<option value="">Duplo clique para selecionar da lista</option>');
					}
					
					if ( json.pflinddelegar == 't' ){
						$('pflinddelegar_s').checked = json.pflinddelegar;
					}else{
						$('pflinddelegar_n').checked = json.pflinddelegar;
					}
					
					if ( json.pflsuperuser == 't' ){
						$('pflsuperuser_s').checked = json.pflsuperuser;
					}else{
						$('pflsuperuser_n').checked = json.pflsuperuser;
					}
					
					if ( json.pflsuporte == 't' ){
						$('pflsuporte_s').checked = json.pflsuporte;
					}else{
						$('pflsuporte_n').checked = json.pflsuporte;
					}
					
					if ( json.pflsncumulativo == 't'){
						$('pflsncumulativo_s').checked = json.pflsncumulativo;
					}else{
						$('pflsncumulativo_n').checked = json.pflsncumulativo;
					}
					
				}
				
			}
		);
	
		$('requisicao').value = 'alterar';
		$('btnacao').value = 'Alterar';
		
	
	}
	
	function excluirPerfil( pflcod ){
		if( confirm("Deseja realmente excluir este perfil?") ){
			window.location.href = caminhoAtualJs + '?modulo=sistema/perfil/cadperfil&acao=I&requisicao=excluir&pflcod=' + pflcod;
		}
	}
	
</script>
<form id="formulario" name="formulario" action="" method="post">
	<input type="hidden" id="requisicao" name="requisicao" value="inserir"/>
	<input type="hidden" id="pflcod" name="pflcod" value=""/>
	<input type="hidden" id="pflresponsabilidade" name="pflresponsabilidade" value="N" > <!-- Sem associação  -->
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr style="display:<?php echo $_SESSION['sisid'] != 4 ? "none" : "" ?>" >
			<td class="SubTituloDireita">Sistema:</td>
			<td>
				<?php
				
					$sisid = $_SESSION['sisid'];
					
					$sql = "SELECT DISTINCT 
								sisid AS codigo, 
								sisdsc AS descricao 
							FROM 
								sistema 
							ORDER BY 
								descricao";
	 				
					$habilSistema = $_SESSION['sisid'] == 4 ? "S" : "N";
					
					$db->monta_combo( "sisid", $sql, $habilSistema, "Selecione o Sistema", "listaPerfis", "", "", "", "S", "sisid" );
					
				?>
					   
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Descrição:</td>
			<td>
				<?php echo campo_texto( 'pfldsc', 'S', 'S', '', 45, 45, '', '', 'left', '', 0, "id='pfldsc'"); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Finalidade:</td>
			<td>
				<?php echo campo_textarea('pflfinalidade','N','S','',96,5,''); ?>
			</td>
		</tr>
		<?php
			$sql = "SELECT DISTINCT 
						modid AS codigo, 
						moddsc AS descricao 
					FROM 
						seguranca.modulo 
					where 
						modstatus='A'
						and sisid = ".$sisid."
					ORDER BY 
						descricao";
			$arrModulos = $db->carregar($sql);
		?>
			<tr id="tr_modulo" style="display:<?php echo $arrModulos ? "" : "none" ?>" >
				<td align='right' class="SubTituloDireita">Módulo:</td>
		       <td id="td_modulo">
					<?php $db->monta_combo( "modid", $arrModulos ? $arrModulos : array(), 'S', "Selecione o Módulo", "", "", "", "", "N", "modid" ); ?>
				</td>
			</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Nível:</td>
	        <td>
		        <?php echo campo_texto( 'pflnivel', 'S', 'S', '', 2, 2, '', '', 'left', '', 0, "id='pflnivel'" ); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Pode Delegar?</td>
	        <td>
		        <input type="radio" name="pflinddelegar" id="pflinddelegar_s" value="t"/> Sim
		        <input type="radio" name="pflinddelegar" id="pflinddelegar_n" value="f"  checked="checked"/> Não
		        <img src="../imagens/obrig.gif" border="0">
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Super Usuário?</td>
	        <td>
		        <input type="radio" name="pflsuperuser" id="pflsuperuser_s" value="t"/> Sim
		        <input type="radio" name="pflsuperuser" id="pflsuperuser_n" value="f"  checked="checked"/> Não
		        <img src="../imagens/obrig.gif" border="0">
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Suporte?</td>
	        <td>
		        <input type="radio" name="pflsuporte" id="pflsuporte_s" value="t"/> Sim
		        <input type="radio" name="pflsuporte" id="pflsuporte_n" value="f" checked="checked"/> Não
		        <img src="../imagens/obrig.gif" border="0">
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Cumulativo?</td>
	        <td>
		        <input type="radio" name="pflsncumulativo" id="pflsncumulativo_s" value="t"/> Sim
		        <input type="radio" name="pflsncumulativo" id="pflsncumulativo_n" value="f" checked="checked"/> Não
		        <img src="../imagens/obrig.gif" border="0">
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Perfil Associado (Filho):</td>
	       <td>
				<?php
				
					$sql = "SELECT DISTINCT 
								pflcod AS codigo, 
								pfldsc AS descricao 
							FROM
								seguranca.perfil
							WHERE
								sisid = {$_SESSION["sisid"]} AND pflstatus = 'A'
							ORDER BY
								pfldsc";
					if($_REQUEST['pflcod']){
						$sql = "SELECT DISTINCT 
									pflcod AS codigo, 
									pfldsc AS descricao 
								FROM
									seguranca.perfil
								WHERE
									sisid = {$_SESSION["sisid"]} AND pflstatus = 'A'
								AND
									pflcod in ( select pflcod from seguranca.perfilpermissao  where  pflcodpai = '{$_REQUEST['pflcod']}')
								ORDER BY
									pfldsc";
						$pflcodfilho = $db->carregar($sqlFilhos);
					}
					
					combo_popup( 'pflcodfilho', $sql, 'Selecione o(s) Perfil(s)', '360x460' );
					
				?>
			</td>
		</tr>
		<tr bgcolor="#CCCCCC">
			<td></td>
			<td>
				<input type="button" name="btnacao" id="btnacao" value="Incluir" onclick="validaInclusao();" class="botao">
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subTituloCentro"> Lista de Perfis </td>
		</tr>
	</table>
</form>
<div id="listaPerfis">
	<?php 
	
		$sql = "SELECT
					'<center>
						<img src=\"/imagens/alterar.gif\" onclick=\"alterarPerfil( ' || pflcod || ' );\" style=\"cursor:pointer;\"/>
						<img src=\"/imagens/excluir.gif\" onclick=\"excluirPerfil( ' || pflcod || ' );\" style=\"cursor:pointer;\"/>
					</center>',
					pflnivel,
					pfldsc,
					pflcod,
					CASE WHEN 
						pflsuperuser = false
					THEN
						'<center><img src=\"/imagens/exclui_p.gif\"/></center>'
					ELSE
						'<center><img src=\"/imagens/check_p.gif\"/></center>'
					END as superusuario,
					CASE WHEN 
						pflinddelegar = false
					THEN
						'<center><img src=\"/imagens/exclui_p.gif\"/></center>'
					ELSE
						'<center><img src=\"/imagens/check_p.gif\"/></center>'
					END as delegar,
					CASE WHEN 
						pflsuporte = false
					THEN
						'<center><img src=\"/imagens/exclui_p.gif\"/></center>'
					ELSE
						'<center><img src=\"/imagens/check_p.gif\"/></center>'
					END as suporte,
					CASE WHEN 
						(pflsncumulativo = false OR pflsncumulativo is null)
					THEN
						'<center><img src=\"/imagens/exclui_p.gif\"/></center>'
					ELSE
						'<center><img src=\"/imagens/check_p.gif\"/></center>'
					END as cumulativo
				FROM
					seguranca.perfil per
				WHERE
					sisid = {$_SESSION["sisid"]} AND pflstatus = 'A'
				ORDER BY
					pflnivel, pfldsc";
		$arrDados = $db->carregar($sql);
		
		if($arrDados){
			foreach($arrDados as $k => $dado){
				$sql = "select 
							pfldsc 
						from 
							seguranca.perfilpermissao pfp
						inner join
							seguranca.perfil per ON per.pflcod = pfp.pflcodfilho
						where 
							pflcodpai = {$dado['pflcod']}";
				$arrPflcodfilho = $db->carregarColuna($sql);
				$arrDados[$k]['pflcodfilho'] = ($arrPflcodfilho ? implode(", ",$arrPflcodfilho) : "N/A");
				unset($arrDados[$k]['pflcod']);
			}
		}
		
		$cabecalho = array( "Ação", "Nível", "Perfil", "Super Usuário", "Pode Delegar", "Suporte", "Cumulativo" ,"Perfil Associado (Filho)");
		$db->monta_lista( $arrDados, $cabecalho, 100, 4, 'N','center', '', '', '', '' );
		
	?>
</div>