<?
$_SESSION['academico']['orgid'] = 1; // Universidade
$academico = new academico();

if( $_REQUEST['carga'] ){
	?>
	<center>
		<fieldset style="background: #fff; width: 95%;">
			<legend align="center">Lista de Campus</legend>
	<?	
		$entid = $_REQUEST['carga'];
		$funid = ACA_ID_CAMPUS; 
			
		$sql = "SELECT 
					'<a style=\"cursor:pointer;\" onclick=\"abreQuestionarioCampus( $entid ,' || e.entid || ');\">' || upper(e.entnome) || '</a>' as nome,
					upper(mun.mundescricao) as municipio, 
					upper(mun.estuf) as uf
				FROM
					entidade.entidade e2
				INNER JOIN entidade.entidade 		e 	ON e2.entid = e.entid
				INNER JOIN entidade.funcaoentidade 	ef  ON ef.entid = e.entid
				INNER JOIN entidade.funentassoc 	ea  ON ea.fueid = ef.fueid 
				LEFT  JOIN entidade.endereco 		ed  ON ed.entid = e.entid
				LEFT  JOIN territorios.municipio 	mun ON mun.muncod = ed.muncod
				WHERE
					ea.entid = {$entid} AND e.entstatus = 'A' AND ef.funid = {$funid}
				ORDER BY
					e.entnome";
		
		$cabecalho = array('Campus', 'Município', 'UF');
		$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '95%');
	?>
		</fieldset>
	</center>
	<?
	die;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$orgid = $_SESSION['academico']['orgid']; //Profissional

// limpa todas as sessões do módulo
unset($_SESSION['academico']);

// Monta as abas com os orgão de responsabilidade do usuário (se nao for superUser)
$res = academico_pega_orgao_permitido();

if ($res[0]["id"] != ''){
	$orgaoPermitido = false;
	foreach($res as $dadosOrg){
		if ($dadosOrg['id'] == $orgid && $orgid != ''){
			$orgaoPermitido = true;
			continue;
		}
	}
	if ( !$orgaoPermitido ){
		$orgid = $res[0]['id'];
	}
	
	$link = 'academico/academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_profissional&acao=A&orgid=' . $orgid;
	$_SESSION['academico']['orgid'] = $orgid;
	
	monta_titulo( 'Programa de Consolidação das IFES - Lista de Instituições', 'Ensino Superior' );

	$arrPerfil = pegaPerfilGeral();
	
	if( in_array(PERFIL_ASSESSORIA_ALTA_GESTAO,$arrPerfil) || 
	    in_array(PERFIL_ALTA_GESTAO,$arrPerfil) || 
	    in_array(PERFIL_SUPERUSUARIO,$arrPerfil) || 
	    in_array(PERFIL_CADASTROGERAL,$arrPerfil) || 
	    in_array(PERFIL_IFESCADBOLSAS,$arrPerfil) || 
	    in_array(PERFIL_IFESCADCURSOS,$arrPerfil)  || 
	    in_array(PERFIL_IFESCADASTRO,$arrPerfil) ) {
		
		switch ( $orgid ){
			case '1':
				$funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
			break;
			case '2':
				$funid = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";
			break;
			case '3':
				$funid = " in ('" . ACA_ID_UNIDADES_VINCULADAS . "')";
			break;
		}
		
		if( in_array(PERFIL_ASSESSORIA_ALTA_GESTAO,$arrPerfil) || 
			in_array(PERFIL_ALTA_GESTAO,$arrPerfil) || 
			$db->testa_superuser() || 
			academico_possui_perfil(PERFIL_ADMINISTRADOR) || 
			academico_possui_perfil_sem_vinculo() || 
			academico_possui_perfil(PERFIL_MECCADASTRO) || 
			academico_possui_perfil_resp_tipo_ensino()){
	
				$sql = "SELECT
							e.entid as codigo,
							entsig || ' - ' || UPPER(entnome) as descricao
						FROM
							entidade.entidade e 
						INNER JOIN
							entidade.funcaoentidade ef ON ef.entid = e.entid
						LEFT JOIN 
							entidade.endereco ed ON e.entid = ed.entid
						WHERE
							e.entstatus = 'A' AND ef.funid ". $funid . "
						GROUP BY e.entid, e.entnome , e.entsig
						ORDER BY
							 e.entsig, e.entnome";
				
			}else{
				
				$sql = "SELECT
							e.entid as codigo,
							entsig || ' - ' || UPPER(entnome) as descricao
						FROM
							entidade.entidade e 
						INNER JOIN
							entidade.funcaoentidade ef ON ef.entid = e.entid
						LEFT JOIN 
							entidade.endereco ed ON e.entid = ed.entid
						INNER JOIN
							academico.usuarioresponsabilidade ur ON ur.entid = e.entid
																	AND ur.rpustatus = 'A'
						WHERE
							e.entstatus = 'A' AND ef.funid ". $funid . " AND ur.usucpf = '{$_SESSION['usucpf']}'
						GROUP BY e.entid, e.entnome,  e.entsig
						ORDER BY
							 e.entsig, e.entnome";
				
			}
			
			$arrDados = $db->carregar($sql);
			
	}
?>
	<form name="formulario" id="pesquisar" method="POST" action="">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
			<tr>
				<td  bgcolor="#CCCCCC" colspan="2"><b>Argumentos da Pesquisa</b></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Instituição</td>
				<td>
					<?php
					
						switch ( $orgid ){
							case '1':
								$funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
							break;
							case '2':
								$funid = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";
							break;
							case '3':
								$funid = " in ('" . ACA_ID_UNIDADES_VINCULADAS . "')";
							break;
						}
							
						$sql_unidade = "SELECT
											e.entid as codigo,
											e.entnome as descricao
										FROM
											entidade.entidade e
										INNER JOIN
											entidade.funcaoentidade ef ON ef.entid = e.entid
										WHERE
											e.entstatus = 'A' AND ef.funid {$funid}
										ORDER BY
											e.entnome";
						$entid = $_REQUEST['entid'] ? $_REQUEST['entid'] : ''; 
						
						$db->monta_combo("entid", $sql_unidade, "S", "Todas as Instituições", '', '', '', '', 'N','entid');
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">UF</td>
				<td>
					<?php
						
						$sql_uf = "SELECT
									estuf as codigo,
									estdescricao as descricao
								FROM
									territorios.estado";
						
						$estuf = $_REQUEST['estuf'] ? $_REQUEST['estuf'] : '';
						
						$db->monta_combo("estuf", $sql_uf, "S", "Todas", '', '', '', '150', 'N','estuf');
						
					?>
				</td>
			</tr>
			<tr>
				<td bgcolor="#CCCCCC"></td>
				<td bgcolor="#CCCCCC">
					<input style="cursor:pointer;" type="submit" value="Pesquisar" />
					<input style="cursor:pointer;" type="button" value="Ver Todos" onCLick="academico_verTodasUnidades();" />
				</td>
			</tr>
		</table>
	</form>
<?php

	// lista as unidades
	$filtro = '';
	$filtro .= !empty( $_REQUEST["entid"] ) ? ' AND e.entid'  . " = {$_REQUEST["entid"]}"    : '';
	$filtro .= !empty( $_REQUEST["estuf"] ) ? ' AND ed.estuf' . " = '{$_REQUEST["estuf"]}'"  : '';
		
	$funid = " in ('" . ACA_ID_UNIVERSIDADE . "')"; // Universidade
	
	$travaUniversidades = '';
	if( !$db->testa_superuser() && 
		!academico_possui_perfil(PERFIL_ADMINISTRADOR) ){
		//$travaUniversidades = " AND e.entid IN (388692,388693,388689,388703,388717)";
		$travaUniversidades = " AND e.entid IN (388693)";
	}
	
	if( $db->testa_superuser() || 
		academico_possui_perfil(PERFIL_ADMINISTRADOR) || 
		academico_possui_perfil_sem_vinculo() || 
		academico_possui_perfil(PERFIL_MECCADASTRO) || 
		academico_possui_perfil_resp_tipo_ensino()){

		$sql = "SELECT
					'<center>
						<img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" 
							 border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || e.entid || '\" 
							 name=\"+\" onclick=\"desabilitarConteudo( ' || e.entid || ' ); formatarParametros();
							 abreconteudo(\'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_profissional&acao=A&subAcao=gravarCarga&orgid={$_REQUEST['orgid']}&carga=' || e.entid || '&params=\' + params, ' || e.entid || ');\"/>
							 </center>' as img,
					CASE WHEN entsig <> '' THEN
						'<a style=\"cursor:pointer;\" onclick=\"abreQuestionarioUnidade( ' || e.entid || ' );\">' || UPPER(entsig) ||  ' - ' || UPPER(entnome) || '</a>'
						ELSE
						'<a style=\"cursor:pointer;\" onclick=\"abreQuestionarioUnidade( ' || e.entid || ' );\">' || UPPER(entnome) ||  '</a>' END as nome,
					upper(mun.mundescricao) as municipio, upper(mun.estuf) as uf,
					'<tr>
						<td style=\"padding:0px;margin:0;\"></td>
						<td id=\"td' || e.entid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td>
						<td style=\"padding:0px;margin:0;\"></td>
					</tr>' as tr
				FROM
					entidade.entidade e 
				INNER JOIN entidade.funcaoentidade 	ef  ON ef.entid = e.entid AND fuestatus = 'A'
				LEFT  JOIN entidade.endereco 		ed  ON ed.entid = e.entid
				LEFT  JOIN territorios.municipio  	mun ON mun.muncod = ed.muncod
				WHERE
					e.entstatus = 'A' $travaUniversidades AND ef.funid ". $funid . $filtro . " 
				GROUP BY e.entid, e.entnome , e.entsig, mun.mundescricao, mun.estuf
				ORDER BY
					 e.entsig, e.entnome";
		
	}else{
		
		$sql = "SELECT
				'<center>
					<img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" 
						 width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || e.entid || '\" name=\"+\" 
						 onclick=\"desabilitarConteudo( ' || e.entid || ' ); formatarParametros();
						 abreconteudo(\'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_profissional&acao=A&subAcao=gravarCarga&orgid={$_REQUEST['orgid']}&carga=' || e.entid || '&params=\' + params, ' || e.entid || ');\"/></center>' as img,
				CASE WHEN entsig <> '' THEN
					'<a style=\"cursor:pointer;\" onclick=\"abreQuestionarioUnidade( ' || e.entid || ' );\">' || UPPER(entsig) ||  ' - ' || UPPER(entnome) || '</a>'
					ELSE
					'<a style=\"cursor:pointer;\" onclick=\"abreQuestionarioUnidade( ' || e.entid || ' );\">' || UPPER(entnome) ||  '</a>' END as nome,
				upper(mun.mundescricao) as municipio, upper(mun.estuf) as uf,
				'<tr>
					<td style=\"padding:0px;margin:0;\"></td>
					<td id=\"td' || e.entid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td>
					<td style=\"padding:0px;margin:0;\"></td>
				</tr>' as tr
			FROM
				entidade.entidade e 
			INNER JOIN entidade.funcaoentidade 	ef  ON ef.entid = e.entid AND fuestatus = 'A'
			LEFT  JOIN entidade.endereco 		ed  ON ed.entid = e.entid
			LEFT  JOIN territorios.municipio 	mun ON mun.muncod = ed.muncod
			INNER JOIN academico.usuarioresponsabilidade ur ON ur.entid = e.entid AND ur.rpustatus = 'A'
			WHERE
				e.entstatus = 'A' $travaUniversidades AND ef.funid ". $funid . $filtro . " AND ur.usucpf = '{$_SESSION['usucpf']}' 
			GROUP BY e.entid, e.entnome,  e.entsig, mun.mundescricao, mun.estuf
			ORDER BY
				 e.entsig, e.entnome";
		
	}
	
	$cabecalho = array('Ação', 'Instituição', 'Município', 'UF');
	$db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '');

}else{ ?>
	
	<?php monta_titulo( 'Programa de Consolidação das IFES', '' ); ?>
	<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
		<tr style="text-align: center; color: #ff0000;">
			<td>
				Usuário sem permissões para visualizar unidades. <br/>
				Favor entrar em contato com o gestor do módulo para verificar as responsabilidades de seu perfil.
			</td>
		</tr>
	</table>

<?php 
} 
?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script>
	
	function academico_verTodasUnidades(){
		var form = window.document.getElementById("pesquisar");
		
		for( var i=0; i < form.elements.length; i++ ){
			var campo = form.elements[i];
			
			switch(campo.type){
				case "select-one":
					campo.options[0].selected = true;
			}
		}
		
		form.submit();
	}

	var params;
	function formatarParametros(){
    	params = Form.serialize($('pesquisar'));
	}

	function desabilitarConteudo( id ){
		var url = window.location+'&carga='+id;
		if ( document.getElementById('img'+id).name == '-' ) {
			url = url + '&subAcao=retirarCarga';
			var myAjax = new Ajax.Request(
				url,
				{
					method: 'post',
					asynchronous: false
				});
		}
	}
	function abreQuestionarioUnidade( entid ){
		return window.location = '?modulo=principal/consolidacaoExpansao/consolidacaoExp_questUni&acao=A&entid=' + entid;
	}
	function abreQuestionarioCampus( entid, entidcampus ){
		return window.location = '?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&entid=' + entid + '&entidcampus=' + entidcampus;
	}
</script>