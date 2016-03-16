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

function pegaNomeDoProjetoAtual( $intAnoExercicioAtual )
{
	return "PPA " . $intAnoExercicioAtual;	
}

function pegaNomeDaAcaoAtual( $intIdAcao )
{
	$strSql = sprintf( "
			 select prgcod || '.' || acacod || '.' || unicod || '.' || loccod || ' - ' || acadsc || '(' || sacdsc || ')' as nome  
			 from monitora.acao 
			 where acaid = %d
		",
			$intIdAcao
	);
	$arrAcao= executaBusca( $strSql );
	$strNome = $arrAcao[ 'nome'];
	$strNome = str_replace( array( chr( 13) , chr( 10 ) , "\n" , "	" ) , array( " " , " " , " " , " " ) , $strNome );
	return $strNome;
	
}

function procuraTarefaProjeto( $intAnoExercicioAtual )
{
	global $db;
	$strSql = sprintf( "
			 select * 
			 from pde.atividade 
			 where atiidpai is null 
			 and atistatus = 'A'
			 and atidescricao = %s
		",
			executaEscape( pegaNomeDoProjetoAtual( $intAnoExercicioAtual ) )
	);

	$arrProjeto = executaBusca( $strSql );
	return $arrProjeto;
}

function procuraTarefaAcao( $intIdAcao , $intAnoExercicioAtual )
{
	global $db;
	$arrProjeto = retornaTarefaProjeto( $intAnoExercicioAtual );

	$strSql = sprintf( "
			 select * 
			 from pde.atividade 
			 where atiidpai = %d
			 and acaid = %d
			 and atistatus = 'A'
			",
			$arrProjeto[ 'atiid' ],
			$intIdAcao,
			executaEscape( pegaNomeDaAcaoAtual( $intIdAcao ) )
	);


/*	$strSql = sprintf( "
		 select at.* 
			 from pde.atividade at
         INNER JOIN pde.planointernoatividade p ON p.atiid = at.atiid
		 inner JOIN financeiro.planointerno pl on pl.pliid = p.pliid
		 inner join monitora.acao a on a.acaid = pl.acaid
		 inner JOIN public.unidade u on u.unicod = a.unicod and u.unitpocod='U'
		 left join financeiro.execucao v on v.plicod=pl.plicod and v.ptres=pl.pliptres
		 where a.acaid = %d
			 and at.atistatus = 'A' 
			 AND at._atiprojeto = 3
			",
			$intIdAcao,
			executaEscape( pegaNomeDaAcaoAtual( $intIdAcao ) )
	);
	*/

	$arrAcao = executaBusca( $strSql );
	return $arrAcao;
	
}

function criaTarefaProjeto( $intAnoExercicioAtual)
{
	global $db;
	
	$arrProjeto = array();
	$arrProjeto[ 'atiidpai' ] = 'null';
	$arrProjeto[ 'atistatus' ] = 'A';
	$arrProjeto[ 'atidescricao' ] = pegaNomeDoProjetoAtual( $intAnoExercicioAtual );
	
	$strSql = sprintf( "
			 insert 
			  into
			 pde.atividade
			(	
			  atiidpai ,
			  atistatus ,
			  atidescricao ,
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

function criaTarefaAcao( $intIdnAcao , $intAnoExercicioAtual )
{
	global $db;
	
	$arrProjeto = retornaTarefaProjeto( $intAnoExercicioAtual );
	
	$arrAcao = array();
	$arrAcao[ 'atiidpai' ]		= $arrProjeto[ "atiid" ];
	$arrAcao[ 'atistatus' ]		= 'A';
	$arrAcao[ 'atidescricao' ]	= pegaNomeDaAcaoAtual( $intIdnAcao );
	$arrAcao[ 'acaid' ]			= $intIdnAcao;
	
	$strSql = sprintf( "
			 insert 
			  into
			 pde.atividade
			(
			  atiidpai ,
			  atistatus ,
			  atidescricao ,
			  acaid,
			  _atiprojeto ,
			  atiordem
			) 
			 values
			(
				%d,
			    %s,
				%s,
				%d,
				%d,
				( select coalesce( max(atiordem), 0 ) + 1 as novaordem from pde.atividade as ativ2 where ativ2.atiidpai = %d and atistatus = %s )
			)
			returning
				atiid
		",
			$arrAcao[ 'atiidpai' ],
			executaEscape( 'A' ),
			executaEscape( $arrAcao[ 'atidescricao' ] ) ,
			$arrAcao[ 'acaid' ] ,
			$arrAcao[ 'atiidpai' ],	
			$arrAcao[ 'atiidpai' ],	
			executaEscape( 'A' )
		);
		
	$arrLinha = executaInsercao( $strSql );
	$arrAcao[ 'atiid' ] = $arrLinha[ 'atiid' ];
	
	atividade_calcular_dados( $arrProjeto[ "atiid" ]);
	return $arrAcao;	
}

function retornaTarefaProjeto( $intAnoExercicioAtual )
{
	$arrProjeto = procuraTarefaProjeto( $intAnoExercicioAtual );
	if( $arrProjeto == null )
	{
		$arrProjeto = criaTarefaProjeto( $intAnoExercicioAtual );
	}
	return $arrProjeto;
}

function retornaTarefaAcao( $intIdnAcao , $intAnoExercicioAtual )
{
	global $db;
	$arrProjeto = retornaTarefaProjeto( $intAnoExercicioAtual );
	if( procuraTarefaAcao( $intIdnAcao , $intAnoExercicioAtual ) == null )
	{
		criaTarefaAcao( $intIdnAcao , $intAnoExercicioAtual );
	}
	return procuraTarefaAcao( $intIdnAcao , $intAnoExercicioAtual );
}

?>