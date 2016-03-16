<?php 

function gravarAssociacao(){
	global $db;
	
	extract( $_POST );
	
	$var = $tipo == 'mun' ? 'muncod' : 'estuf';

	$sql = "SELECT 
				d.docid 
			FROM 
				workflow.documento d
			INNER JOIN par.instrumentounidade iu ON iu.docid = d.docid AND iu.{$var} = '{$local}'
			WHERE 
				d.esdid = 315";
	
	if( $db->pegaUm( $sql ) == '' ){
		echo 'erro';
		die;
	}
	
	$sql = "SELECT aasid FROM par.analistasassociados WHERE usucpf = '{$cpf}' AND pflcod = {$pflcod} AND {$var} = '{$local}'";
	$aasid = $db->pegaUm( $sql );
	
	if( $aasid != '' ){
		$sql = "DELETE FROM par.analistasassociados WHERE aasid = ".$aasid;
	}else{
		$sql = "INSERT INTO par.analistasassociados ( usucpf, pflcod, $var, data, usucpfassociador ) VALUES ( '{$cpf}', {$pflcod},'{$local}',NOW(),'{$_SESSION['usucpf']}' )";
	}
	$db->executar( $sql );
	$db->commit();
}

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	die;
}

$sql = "SELECT DISTINCT 
			ur.usucpf as cpf,
			usu.usunome as nome,
			p.pflcod as codigo, 
			p.pfldsc as perfil,
			'' as associar
	--		ur.estuf as estado,
	--		ur.muncod as municipio
		FROM
			par.usuarioresponsabilidade ur
		INNER JOIN seguranca.usuario usu ON usu.usucpf = ur.usucpf
		INNER JOIN seguranca.perfil p ON p.pflcod = ur.pflcod
		WHERE
			ur.pflcod IN ( ".PAR_PERFIL_EQUIPE_TECNICA.", ".PAR_PERFIL_EQUIPE_FINANCEIRA." ) 
			AND ur.rpustatus = 'A'";

$dados = $db->carregar( $sql );
$arr = array();

if( is_array($dados) ){
	foreach( $dados as $x => $dado ){
		$arr[$x]['acao']    = $dado['cpf'];
		$arr[$x]['nome']    = $dado['nome'];
		$arr[$x]['perfil']  = $dado['perfil'];
		
		$sql = "SELECT 
					CASE WHEN ur.muncod IS NOT NULL THEN
						mun.mundescricao || ' - ' || mun.estuf || '<input type=\"checkbox\" id=\"checa\" ' || CASE WHEN aas.aasid IS NOT NULL THEN 'checked=\"checked\"' ELSE '' END || ' onclick=\"gravaAssociacao( \'{$dado['cpf']}\', {$dado['codigo']},  \'' || mun.muncod || '\', \'mun\', this)\">'
					ELSE
						est.estdescricao || '<input type=\"checkbox\" id=\"checa\" ' || CASE WHEN aas2.aasid IS NOT NULL THEN 'checked=\"checked\"' ELSE '' END || ' onclick=\"gravaAssociacao( \'{$dado['cpf']}\', {$dado['codigo']} , \'' || est.estuf || '\', \'est\', this)\">'
					END as descricao
				FROM
					par.usuarioresponsabilidade ur
				LEFT JOIN territorios.municipio mun ON mun.muncod = ur.muncod
				LEFT JOIN territorios.estado est ON est.estuf = ur.estuf
				LEFT JOIN par.analistasassociados aas ON aas.muncod = mun.muncod AND aas.usucpf = '{$dado['cpf']}' AND aas.pflcod = {$dado['codigo']} 
				LEFT JOIN par.analistasassociados aas2 ON aas2.estuf = est.estuf AND aas2.usucpf = '{$dado['cpf']}' AND aas2.pflcod = {$dado['codigo']} 
				WHERE
					ur.usucpf = '{$dado['cpf']}' AND ur.rpustatus = 'A' AND ur.pflcod = ".$dado['codigo'];
	
		$desc = $db->carregarColuna( $sql );
		
		$arr[$x]['associa'] = implode('<br>',$desc); 
	}
}

/************************* CABEÇALHO E TÍTULO ******************************/
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( 'Vinculação de Analistas', '' );
?>
<head>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<script type="text/javascript">
		function visualizarSubacao(sbaid){
			var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid;
			window.open(local, 'popupSubacao', "height=650,width=900,scrollbars=yes,top=50,left=200" );
		}
		function gravaAssociacao( cpf, pflcod, local, tipo, este ){
			if( tipo == 'mun' ){
				variavel = 'Município';
			} else {
				variavel = 'Estado';
			}			
			$.ajax({
				type: "POST",
				url: window.location,
				data: "requisicaoAjax=gravarAssociacao&cpf=" + cpf + "&pflcod=" + pflcod + "&local=" + local + "&tipo=" + tipo,
				success: function(msg){
					if( msg == 'erro' ){
						alert('Você não pode vincular este ' + variavel + ' pois ele não está em Análise.');
						este.checked = false;
					}
				}
		 	});
		}
	</script>
</head>
	<body>
		<?php
			$cabecalho = array("Ação","Nome do Analista","Perfil","Associar");
			$db->monta_lista($arr,$cabecalho,501,5,"N","100%","S");
//			$db->monta_lista($sql,$cabecalho,501,5,"N","100%","S");
		?>
	</body>
</html>