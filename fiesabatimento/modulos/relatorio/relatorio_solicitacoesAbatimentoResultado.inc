<?php
// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = recuperarSql();
$coluna    = recuperarArColunas();
$agrupador = recuperarArAgrupador();
$dados 	   = $db->carregar( $sql );

$rel->setColuna( $coluna );
$rel->setAgrupador( $agrupador, $dados ); 

$rel->setEspandir( true );
$rel->setTotNivel( true );
$rel->setTotalizador( false );
//$rel->setMonstrarTolizadorNivel( TIPO_TOTALIZADOR_SUB_ITEM );

function recuperarSql(){
	
	extract($_REQUEST);
	
	$arWhere = array();
	if ( $estuf[0] != '' ){
		array_push( $arWhere, " mun.estuf in ('".implode("','",$estuf)."')" );
	}
	if ( $muncod[0] != '' ){
		array_push( $arWhere, " mun.muncod in ('".implode("','",$muncod)."')" );
	}
	if ( $esfera[0] != '' ){
		array_push( $arWhere, " atp.esferaprofessor in ('".implode("','",$esfera)."')" );
	}
	if ( $esdid[0] != '' ){
		array_push( $arWhere, " doc.esdid in ('".implode("','",$esdid)."')" );
	}
	if ( $atpinep[0] != '' ){
		array_push( $arWhere, " atp.atpinep in ('".implode("','",$atpinep)."')" );
	}
	if ( $sbadatasolicitacao_inicio != '' ){
		$data = formata_data_sql($sbadatasolicitacao_inicio);
		array_push( $arWhere, " sba.sbadatasolicitacao >= '$data'" );
	}
	if ( $sbadatasolicitacao_fim != '' ){
		$data = formata_data_sql($sbadatasolicitacao_fim);
		array_push( $arWhere, " sba.sbadatasolicitacao <= '$data'" );
	}
	if ( $htddata_inicio != '' ){
		$data = formata_data_sql($htddata_inicio);
		array_push( $arWhere, " hst.htddata >= '$data'" );
	}
	if ( $htddata_fim != '' ){
		$data = formata_data_sql($htddata_fim);
		array_push( $arWhere, " hst.htddata <= '$data'" );
	}
	if ( $tipo ){
		if($tipo == '1'){
			array_push( $arWhere, " sba.preid = 1 " );
		}else{
			array_push( $arWhere, " sba.preid is null " );
		}
	}
	
	$stWhere = $arWhere[0] != '' ? ' WHERE ' . implode( " AND ", $arWhere ) : "";
	
	$stORDERBY = $agrupador != '' ? ' ORDER BY ' . implode( " , ", $agrupador ) : "";
	
	$sql = "SELECT DISTINCT
				--agrupadores
				mun.estuf,
				mun.muncod,
				mun.estuf as uf,
				mun.mundescricao as municipio,
				--colunas
				replace(to_char(idocpf::numeric, '000:000:000-00'), ':', '.') as cpf,
				idonome as docente,
				fiesabatimento.agrupaUFMunEscola( sba.sbaid ) as escolas,
				to_char(sbadatasolicitacao,'DD/MM/YYYY') as dt_solicitacao,
				esddsc as estado,
				to_char(hst.htddata,'DD/MM/YYYY') as dt_tramitacao,
				sbaqmtsolicitado as qmt_solicitado,
				sbaqmtaprovado as qmt_aprovado,
				sbaqmtvalido as qmt_valido,
				1 as qtd
			FROM
				fiesabatimento.identificacaodocente ido
			INNER JOIN
				(
				SELECT
					max(sbaid) as sbaid,
					idoid
				FROM
					fiesabatimento.solicitacaoabatimento
				GROUP BY
					idoid
				) 											sba1 ON sba1.idoid = ido.idoid
			INNER JOIN fiesabatimento.solicitacaoabatimento sba ON sba.sbaid = sba1.sbaid
			INNER JOIN fiesabatimento.atuacaoprofissional 	atp ON atp.sbaid = sba1.sbaid
			INNER JOIN territorios.municipio 				mun ON mun.muncod = ido.muncodprofessor
			INNER JOIN workflow.documento 					doc ON doc.docid = sba.docid
			INNER JOIN workflow.estadodocumento 			esd ON esd.esdid = doc.esdid
			LEFT  JOIN 
				(
				SELECT
					max(hstid) as hstid,
					docid
				FROM
					workflow.historicodocumento 
				GROUP BY
					docid
				) 											hst1 ON hst1.docid = doc.docid
			LEFT  JOIN workflow.historicodocumento 			hst ON hst.hstid = hst1.hstid
			$stWhere
			$stORDERBY";
			
	return $sql;
}

function recuperarArColunas(){

	$agrupador 	= $_REQUEST['agrupador'];
	
	$coluna = Array(
				Array( "campo" 	  => "cpf",
					   "type" 	  => "text",
			   		   "label" 	  => "CPF" ),
				Array( "campo" 	  => "escolas",
					   "type" 	  => "text",
			   		   "label" 	  => "Escolas" ),
				Array( "campo" 	  => "dt_solicitacao",
					   "type" 	  => "text",
			   		   "label" 	  => "Data da Solicitação" ),
				Array( "campo" 	  => "estado",
					   "type" 	  => "text",
			   		   "label" 	  => "Estado Atual da Solicitação" ),
				Array( "campo" 	  => "dt_tramitacao",
					   "type" 	  => "text",
			   		   "label" 	  => "Data do Ultimo Trâmite" ),
				Array( "campo" 	  => "qmt_solicitado",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Quantidade de Mêses Solicitados" ),
				Array( "campo" 	  => "qmt_aprovado",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Quantidade de Mêses Aprovados" ),
				Array( "campo" 	  => "qmt_valido",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Quantidade de Mêses Válidos" ),
				Array( "campo" 	  => "qtd",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Quantidade de Docentes" ));
	
	
	return $coluna;
	
}

function recuperarArAgrupador(){
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
										 "cpf",
										 "qtd",
										 "escolas",
										 "dt_solicitacao",
										 "estado",
										 "dt_tramitacao",
										 "qmt_solicitado",
										 "qmt_aprovado",
										 "qmt_valido"
										  )
				);
				
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "estuf":
				array_push($agp['agrupador'], array(
													"campo" => "estuf",
											  		"label" => "UF")									
									   				);
				break;
			case "muncod":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município"									
									   				) );
				break;
		}
	}
	array_push($agp['agrupador'], array(
										"campo" => "docente",
								  		"label" => "Docente"									
						   				) );

	return $agp;
}

?>
<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<div align="center" style="text-align:center;">
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</div>
		<!--  Monta o Relatório -->
		<? echo $rel->getRelatorio(); ?>
	</body>
</html>