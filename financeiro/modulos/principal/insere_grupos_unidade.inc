<?php

// Altera os dados do banco utilizado na página
$servidor_bd = $servidor_bd_siafi;
$porta_bd    = $porta_bd_siafi;
$nome_bd     = $nome_bd_siafi;
$usuario_db  = $usuario_db_siafi;
$senha_bd    = $senha_bd_siafi;

$db2 = new cls_banco();

if ( $_REQUEST['ajax'] == 1 ){

	$uos_selecionadas = str_replace('\\', '', $_REQUEST['uo']);
	
	$sql = "";
	$sql = $db2->carregar("SELECT
							unidsc 
						  FROM 
						  	dw.uo 
						  WHERE 
						  	unicod in (" . $uos_selecionadas . ") AND
						  	guoid IS NOT NULL");
	
	
	echo count($sql);
	die;
}

/**
 * Função que cadastra os novos grupos e atualiza as UO's
 *
 * @param array $dados
 * @author Fernando A. Bagno da Silva
 * @since 02/02/2009
 */
function cadastra_grupo_uo( $dados ){
	
	global $db, $db2;
	
	// Verifica os grupos inseridos no banco
	$guoid = $db2->carregar( "SELECT guoid FROM dw.grupouo 
							 WHERE guostatus ='A'" );
	
	// Se não existir grupo cadastrado, insere direto o novo grupo
	if( empty( $guoid ) ){
		
		// Insere o grupo
		$guoid = $db2->pegaUm( "INSERT INTO dw.grupouo( guoid, 
													   guodsc, 
													   guoabrev, 
													   guostatus ) 
							   VALUES ( {$dados['guoid']},
				 					    '{$dados['guodsc']}',
									    '{$dados['guoabrev']}',
									    'A' ) returning guoid" );
		
	}else {
		
		// Se já existirem os grupos, verifica se o novo dado já existe no banco
		for ( $i = 0; $i < count($guoid); $i++ ){
			
			if ( !in_array($dados['guoid'], $guoid[$i]) ){
				
				// Se não existir grupo com o ID informado, insere o novo registro
				$guoid = $db2->pegaUm( "INSERT INTO dw.grupouo( guoid, 
															   guodsc, 
															   guoabrev, 
															   guostatus ) 
									   VALUES ( {$dados['guoid']},
						 					    '{$dados['guodsc']}',
											    '{$dados['guoabrev']}',
											    'A' ) returning guoid" );

			}else{
				
				// Caso o código do grupo informado já esteja no banco, exibe erro
				echo '<script>
						alert("Já existe um Grupo cadastrado com o Código informado!");
					  </script>';
				return false;
				
			}
		}
	}
		
	// Pega todas as UO's que já possuem grupos atribuidos
	$uo = $db2->carregar( "SELECT 
							unicod,
							unidsc 
						 FROM 
						  	dw.uo 
						  WHERE 
						  	guoid IS NOT NULL" );
	
	// Se não existirem UO's com grupos atribuidos 
	if ( empty($uo) ){
		
		// Atualiza as UO's inseridos os seus respectivos grupos
		foreach( $dados['uo'] as $valor ){
			$sql = $db2->executar( "UPDATE dw.uo SET guoid = {$guoid} 
								    WHERE unicod = '{$valor}'" );
		}
		
	}else {
		
		foreach( $dados['uo'] as $valor ){
			for ( $i = 0; $i < count($uo); $i++ ){
				if ( !in_array( $uo[$i]["gr_unidade_orcamentaria"], $dados['uo'] ) ){
					$sql = $db2->executar( "UPDATE dw.uo SET guoid = {$guoid} 
										   WHERE unicod = '{$valor}'" );
				}		
			}
		}
		
	}

	$db2->commit();
	$db2->sucesso( 'principal/consulta_grupos_unidade', 'A');

}


/**
 * Enter description here...
 *
 * @param unknown_type $dados
 */
function altera_grupo_uo( $dados ){
	
	global $db, $db2;
	
	$sql = $db2->executar( " UPDATE 
								dw.grupouo 
							SET
								guoabrev = '{$dados['guoabrev']}',
								guodsc = '{$dados['guodsc']}'
							WHERE
								guoid = {$dados['guoid']}");
	
	// Atualiza as UO's inseridos os seus respectivos grupos
	if ( count($dados['uo']) > 1){
		
		$sql = $db2->executar( "UPDATE dw.uo SET guoid = null 
							   WHERE guoid = {$dados['guoid']}" );
		
		foreach( $dados['uo'] as $valor ){
			$sql = $db2->executar( "UPDATE dw.uo SET guoid = {$dados['guoid']} 
								    WHERE unicod = '{$valor}'" );
		}
		
	}
	
	$db2->commit();
	$db2->sucesso('principal/consulta_grupos_unidade','A');
	
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

$conexao = array(
	'servidor_bd' => $servidor_bd_siafi,
	'porta_bd' 	  => $porta_bd_siafi,
	'nome_bd' 	  => $nome_bd_siafi,
	'usuario_db'  => $usuario_db_siafi,
	'senha_bd' 	  => $senha_bd_siafi
	);

$hab_guoid = 'S';

if ( $_REQUEST['guoid'] ){
	
	$guoid = $_REQUEST['guoid'];
	$hab_guoid = 'N';
	
	$sql = "";
	$sql = " SELECT 
				gr.guoid,
				gr.guoabrev,
			 	gr.guodsc,
			 	gu.unidsc
			 FROM
			 	dw.grupouo gr
			 INNER JOIN
			 	dw.uo gu ON gr.guoid = gu.guoid 
			 WHERE
			 	gr.guoid = {$_REQUEST['guoid']}";
	
	$dados = $db2->carregar( $sql );
	
	$guoabrev = $dados[0]['guoabrev'];
	$guodsc   = $dados[0]['guodsc'];
	$arrayUos = array();
	
	for( $i = 0; $i < count($dados); $i++ ){
		$arrayUos[$i] = $dados[$i]['unidsc'];
	}
}

// Cadastra e atualiza o grupo de uo's
if ( $_REQUEST['submetido'] == 1 ){
	
	switch( $_REQUEST['requisicao'] ){
		case 'inserir';
			cadastra_grupo_uo( $_REQUEST );
			break;
		case 'alterar';
			altera_grupo_uo( $_REQUEST );
			break;
	}
	
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Módulo Financeiro', 'Gerenciamento Dinâmico de Grupos de Unidades Orçamentárias' );

?>

<script src="/includes/prototype.js"></script>
<script type="text/javascript">

function valida_uos_associadas( form, id ){
	
	var url = '?modulo=principal/insere_grupos_unidade&acao=A&ajax=1';
	var parametros = "&uo=" + id;
	
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				if ( resp.responseText > 1){
					if ( confirm('Algumas das unidades selecionadas já possuem um grupo associado, deseja continuar?')){
						selectAllOptions(form.uo);
						form.submit();
					}
				}else{
					selectAllOptions(form.uo);
					form.submit();
				}
			}
		}
	);
}

	/**
	 * Função que valida o formulário de cadastro de Grupos de UO's.
	 */
	function valida_grupo_uo( form, tipo ){
		
		guoid = form.guoid.value;
		
		if ( guoid == '' ){
			alert('Favor preencher o campo Código do Grupo!');
			return false;
		}
		
		var uos = form.uo;
		var arUos = new Array();
		
		for ( i = 0; i < uos.options.length; i++  ){
			arUos[i] = "'" + uos.options[i].value + "'";
		}
		
		form.requisicao.value = tipo;
		form.guoid.disable = false;
		
		valida_uos_associadas( form, arUos );
			
	}

</script>

<form name="formulario" id="formulario" method="post" action="">
	<input type="hidden" name="submetido" id="submetido" value="1" />
	<input type="hidden" name="requisicao" id="requisicao" value="" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="SubTituloDireita">Código do Grupo</td>
			<td>
				<?php
					echo campo_texto( 'guoid', 'S', $hab_guoid, '', 10, 10, '', '', 'left', '',0, 'id="guoid"' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição Abreviada</td>
			<td>
				<?php
					echo campo_texto( 'guoabrev', 'N', 'S', '', 50, 100, '', '' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição</td>
			<td>
				<?php
					echo campo_texto( 'guodsc', 'N', 'S', '', 50, 250, '', '' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade Orçamentária</td>
			<td>
				<?php
					$sql_uo = "";
					$sql_uo = "SELECT
									unicod as codigo,
									unidsc as descricao
								FROM
									dw.uo
								ORDER BY
									unidsc";
					
					if ( $_REQUEST['guoid'] ){
						
						$sql = "";
						$sql = "SELECT
									unicod as codigo,
									unidsc as descricao
								FROM
									dw.uo
								WHERE
									unidsc in ('".implode("','", $arrayUos)."')";
						
						$uo = $db2->carregar( $sql );
						
					}
					
					combo_popup( 'uo', $sql_uo, 'Selecione a(s) Unidade(s) Orçamentária(s)', '400x400', 0, array(), '', 'S', true, false, 10, 400, null, null, $conexao );
				?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Salvar" style="cursor: pointer;" onclick="valida_grupo_uo(getElementById('formulario'), '<?php if ( !empty($_REQUEST['guoid']) ){ echo 'alterar'; }else{ echo 'inserir';} ?>');" />
				<input type="button" value="Voltar" style="cursor: pointer;" onclick="history.back(-1);"/>
			</td>
		</tr>
	</table>
</form>