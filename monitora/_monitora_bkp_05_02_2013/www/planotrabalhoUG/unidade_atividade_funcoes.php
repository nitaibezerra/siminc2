<?php

function executaEscape( $strTexto )
{
	global $db;
	return $db->escape( $strTexto );
}

function executaBusca( $strSql )
{
	global $db;
//	dbg( $strSql );
	$arrLinha = $db->pegaLinha( $strSql );
//	dbg( $arrLinha );
	return $arrLinha;
}

function executaInsercao( $strSql )
{
	global $db;
//	dbg( $strSql , 1);
	$objResultado = $db->executar( $strSql );
	$arrLinha = pg_fetch_assoc($objResultado);
//	dbg( $arrLinha );
	return $arrLinha;
}

function pegaNomeDoProjetoAtual( $intAnoExercicioAtual ) {
	return "PPA " . $intAnoExercicioAtual;	
}

/*
 * criando o nome da unidade (cod - descriчуo)
 */
function pegaNomeDaUnidadeAtual( $ungcod ) {
	$strSql = "SELECT CASE WHEN (ungcod = '26101') THEN ungcod ELSE unicod END  || ' - ' || ungdsc as nome FROM public.unidadegestora WHERE ungcod = '".$ungcod."'";
	$arrUnidade= executaBusca( $strSql );
	$strNome = $arrUnidade[ 'nome'];
	$strNome = str_replace( array( chr( 13) , chr( 10 ) , "\n" , "	" ) , array( " " , " " , " " , " " ) , $strNome );
	return $strNome;
	
}
/*
 * procura se possui projeto no ano
 */
function procuraTarefaProjeto( $intAnoExercicioAtual )
{
	global $db;
	$strSql = sprintf("SELECT * FROM pde.atividade WHERE atiidpai IS NULL AND atistatus = 'A' AND atidescricao = %s",
					  executaEscape( pegaNomeDoProjetoAtual( $intAnoExercicioAtual ) ));
	$arrProjeto = executaBusca( $strSql );
	return $arrProjeto;
}
/*
 * procura a unidade ( atividade raiz dentro de uma unidade )
 */
function procuraTarefaUnidade( $ungcod , $intAnoExercicioAtual ) {
	global $db;
	$arrProjeto = retornaTarefaProjeto( $intAnoExercicioAtual, $ungcod );

	$strSql = "SELECT * FROM pde.atividade WHERE atiidpai = '".$arrProjeto[ 'atiid' ]."' AND ungcod = '".$ungcod."' AND atistatus = 'A' ORDER BY atiid ASC";
	$arrUnidade = executaBusca( $strSql );
	return $arrUnidade;
	
}

function criaTarefaProjeto( $intAnoExercicioAtual, $ungcod)
{
	global $db;
	
	$arrProjeto = array();
	$arrProjeto[ 'atiidpai' ] = 'null';
	$arrProjeto[ 'atistatus' ] = 'A';
	$arrProjeto[ 'ungcod' ] = $ungcod;
	$arrProjeto[ 'atidescricao' ] = pegaNomeDoProjetoAtual( $intAnoExercicioAtual );
	
	$strSql = sprintf( "
			 insert 
			  into
			 pde.atividade
			(	
			  atiidpai ,
			  atistatus ,
			  atidescricao ,
			  ungcod ,
			  atiordem
			)
			 values
			(	
				%s,
			    %s,
				%s,
				%d
			)
			returning
			 atiid
		",
			$arrProjeto[ 'atiidpai' ],
			executaEscape( $arrProjeto[ 'atistatus' ] ) ,
			executaEscape( $arrProjeto[ 'atidescricao' ] ),
			executaEscape( $arrProjeto[ 'ungcod' ] ),
			0
		);
		
	$arrLinha = executaInsercao( $strSql );
	$arrProjeto[ 'atiid' ] = $arrLinha[ 'atiid' ];
	
	$strSql = sprintf( "
			 update
				 pde.atividade
			 set
			 	 _atiprojeto = %d
			 where
				 atiid = %d 
			",
			$arrProjeto[ 'atiid' ],
			$arrProjeto[ 'atiid' ]
		);
		
	$arrProjeto[ '_atiprojeto' ] = $arrProjeto[ 'atiid' ];
	
	$arrLinha = executaBusca( $strSql );
	atividade_calcular_dados( $arrProjeto[ 'atiid' ] );
	return $arrProjeto;	
}

function criaTarefaUnidade( $ungcod , $intAnoExercicioAtual )
{
	global $db;
	
	$arrProjeto = retornaTarefaProjeto( $intAnoExercicioAtual, $ungcod );
	
	$arrUnidade = array();
	
	$arrUnidade[ 'atiidpai' ]		= $arrProjeto['atiid'];
	$arrUnidade[ 'atistatus' ]		= 'A';
	$arrUnidade[ 'atidescricao' ]	= pegaNomeDaUnidadeAtual( $ungcod );
	$arrUnidade[ 'ungcod' ]			= $ungcod;
	
	$strSql = "INSERT 
			   INTO pde.atividade 
			   (
			   		atiidpai ,
			  		atistatus ,
			  		atidescricao ,
				    ungcod,
			  		_atiprojeto ,
			  		atiordem
			   ) 
			   VALUES
			   (
				'".$arrUnidade[ 'atiidpai' ]."',
			    ".executaEscape( 'A' ).",
				".executaEscape( $arrUnidade[ 'atidescricao' ] ).",
				'".$arrUnidade[ 'ungcod' ]."',
				'".PROJETO."',
				( select coalesce( max(atiordem), 0 ) + 1 as novaordem from pde.atividade as ativ2 where ativ2.atiidpai = '".$arrUnidade[ 'atiidpai' ]."' and atistatus = ".executaEscape( 'A' )." )
				)
			returning
				atiid";
		
	$arrLinha = executaInsercao( $strSql );
	$arrUnidade[ 'atiid' ] = $arrLinha[ 'atiid' ];
	
	atividade_calcular_dados( $arrProjeto[ "atiid" ]);
	return $arrUnidade;	
}

function retornaTarefaProjeto( $intAnoExercicioAtual, $ungcod ) {
	$arrProjeto = procuraTarefaProjeto( $intAnoExercicioAtual );
	if( $arrProjeto == null ) {
		$arrProjeto = criaTarefaProjeto( $intAnoExercicioAtual, $ungcod );
	}
	return $arrProjeto;
}

function retornaTarefaUnidade( $ungcod , $intAnoExercicioAtual )
{
	global $db;
	$arrProjeto = retornaTarefaProjeto( $intAnoExercicioAtual, $ungcod );
	if( procuraTarefaUnidade( $ungcod , $intAnoExercicioAtual ) == null ) {
		criaTarefaUnidade( $ungcod , $intAnoExercicioAtual );
	}
	return procuraTarefaUnidade( $ungcod , $intAnoExercicioAtual );
}

?>