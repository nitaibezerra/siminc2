<?
// Codificação do relatório
header( 'Content-Type: text/html; charset=iso-8859-1' );
// Objeto da classe do relatório dos dados da unidade
$obRelatorio = new relatorioNovasTurmasResultado();
// Verifica se é um relatório p o browser ou excel
$isXls = ($_REQUEST['requisicao'] == 'relatorio_xls') ? true :false;

// Classe responsável por montar o resultado do relatorio
class relatorioNovasTurmasResultado
{
	
	// Variáveis da classe
	public $esfera 			= '';
	public $arrTipoEntidade = array();
	public $requisicao 		= '';
	public $arrColunas 		= array();
	public $arrUfs 			= array();
	public $arrMunicipios 	= array();
	public $dados			= array();
	public $db				= '';
	
	// Método construtor
	function __construct()
	{
		// Instancia a classe do BD
		$this->db = new cls_banco();
		
		set_time_limit(0);

		// Verifica a requisição (relatorio ou relatorio_xls)
	    $this->requisicao = $_POST['requisicao']; // 
	    // Verifica as colunas escolhidas para o relatório
		$this->arrColunas = $this->_POST['coluna'];
	
	}
	
	/**
	 * Monta a string apartir do array e retorna no formato do sql
	 * */
	public function getArrToStrParam( $arrParam )
	{
		if( $arrParam[0] != '')
		{
			foreach( $arrParam as $k => $v )
			{
				$arrParam[$k] = "'{$v}'";
			}
			
			$strReturn = implode(",", $arrParam);
		}
		else
		{
			$strReturn = false;
		}    	
		return $strReturn;		
    }
    
	function monta_sql()
	{
		
		// Carrega o tipo (TURMA OU MUNICIPIO)
		$tipo = $_POST['tipo'];

		// Carrega as colunas
		$arrColunas = $_POST['coluna'];
		// Recupera array com as ufs
		$arrUf= $_POST['estuf'];
		// Monta o parametro para a clausula where
		$strUf = $this->getArrToStrParam($arrUf);
		// Recupera array com os município
    	$arrMunCod = $_POST['muncod'];
    	// Monta o parametro para a clausula where 		
    	$strMunCod= $this->getArrToStrParam( $arrMunCod );
    	// Ano exercício
    	$anoExercicio = (int)$_SESSION['exercicio'];
    	// Ano censo
		$anoCenso 	= ((int)$_SESSION['exercicio'] - 1);
    	
		// Caso 1 = Turma 2= município
		if( $tipo == 1 )
		{
			// Carrega situação 
    		$situacao = $_POST['situacao_turma'];
			if(is_array($arrColunas) && ( count($arrColunas) > 0 ))
			{
				foreach( $arrColunas as $k => $col )
				{
					switch ($col) {
					    	
	
					    case 'T_NOMTURM'://Nome da Turma
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turma.turdsc', 
					        	'nomeColuna' => '"Nome da Turma"'
					        );
					        break;
					    case 'T_CINEP': //Código INEP
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turma.entcodent', 
					        	'nomeColuna' => '"Código INEP"'
					        );
					        break;
						         
					    case 'T_TREDE'://Tipo de rede
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turma.tirdescricao', 
					        	'nomeColuna' => '"Tipo de rede"'
					        );
					        break;
					    case 'T_TATEND': //Tipo atendimento
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turma.tipoatendimento', 
					        	'nomeColuna' => '"Tipo atendimento"'
					        );
					        break;
					    case 'T_CADEDUCACENSO': //Está cadastrado no Educacenso 
					    	$arrayCols[] = array(
					        	'coluna'	 => 'CASE WHEN turma.turcadeducacenso THEN \'Sim\' ELSE \'Não\' END', 
					        	'nomeColuna' => '"Está cadastrado no Educacenso"'
					        );
					        break;
	 
						case 'T_DTINITURM': //Data início da Turma
					        $arrayCols[] = array(
					        	'coluna'	 => 'TO_CHAR(turma.turdtinicio, \'DD/MM/YYYY\')', 
					        	'nomeColuna' => '"Data início da Turma"'
					        );
					        break;
					    case 'T_MESCADTURM': //Mês de cadastro da Turma 
					    	$arrayCols[] = array(
					        	'coluna'	 => 'case when turma.turmes = 1 then \'Janeiro\' 
													when turma.turmes = 2 then \'Fevereiro\'
													when turma.turmes = 3 then \'Março\' 
													when turma.turmes = 4 then \'Abril\' 
													when turma.turmes = 5 then \'Maio\' 
													when turma.turmes = 6 then \'Junho\' 
													when turma.turmes = 7 then \'Julho\' 
													when turma.turmes = 8 then \'Agosto\' 
													when turma.turmes = 9 then \'Setembro\' 
													when turma.turmes = 10 then \'Outubro\' 
													when turma.turmes = 11 then \'Novembro\' 
													when turma.turmes = 12 then \'Dezembro\' 
												end||\'/\'||turma.turano', 
					        	'nomeColuna' => '"Mês de cadastro da Turma"'
					        );
					        break;
					    case 'T_DTENVANALISE': //Data Envio para Análise
					    	$arrayCols[] = array(
					        	'coluna'	 => 'TO_CHAR(to_date(turma.dtanalise, \'YYYY-MM-DD\'), \'DD/MM/YYYY\')', 
					        	'nomeColuna' => '"Data Envio para Análise"'
					        );
					        break;
					    case 'T_QTDMESESREPASSE': //QTD de meses para repasse dos recursos 
					    	$arrayCols[] = array(
					        	'coluna'	 => '(12 - (to_number(substring(turma.dtanalise from 6 for 2), \'99\' ) -1 ))', 
					        	'nomeColuna' => '"QTD de meses para repasse dos recursos"'
					        );
					        break;
					    case 'T_QTDALUCREINT': //QTD alunos em creche tempo integral 
					    	$arrayCols[] = array(
					        	'coluna'	 => 'qtdalunoscrecheintegral', 
					        	'nomeColuna' => '"QTD alunos em creche tempo integral"'
					        );
					        break;
					    case 'T_QTDALUCREPAR': //QTD alunos em creche tempo parcial
					    	$arrayCols[] = array(
					        	'coluna'	 => 'qtdalunoscrecheparcial', 
					        	'nomeColuna' => '"QTD alunos em creche tempo parcial"'
					        );
					        break;
					    case 'T_QTDALUPREINT': //QTD alunos pré-escola integral
					    	$arrayCols[] = array(
					        	'coluna'	 => 'qtdalunospreescolaintegral', 
					        	'nomeColuna' => '"QTD alunos pré-escola integral"'
					        );
					        break;
					    case 'T_QTDALUPREPAR': //QTD alunos pré-escola parcial
					    	$arrayCols[] = array(
					        	'coluna'	 => 'qtdalunospreescolaparcial', 
					        	'nomeColuna' => '"QTD alunos pré-escola parcial"'
					        );
					        break;
					    case 'T_QTDPROFPREINT': //QTD professores pré-escola integral
					    	$arrayCols[] = array(
					        	'coluna'	 => 'qtdprofessorpreescolaintegral', 
					        	'nomeColuna' => '"QTD professores pré-escola integral"'
					        );
					        break;
					    case 'T_QTDPROFPREPAR': //QTD professores pré-escola parcial
					    	$arrayCols[] = array(
					        	'coluna'	 => 'qtdprofessorpreescolaparcial', 
					        	'nomeColuna' => '"QTD professores pré-escola parcial"'
					        );
					        break;
					    case 'T_QTDPROFCREINT': //QTD de professores em creche tempo integral
					    	$arrayCols[] = array(
					        	'coluna'	 => 'qtdprofessorcrecheintegral', 
					        	'nomeColuna' => '"QTD de professores em creche tempo integral"'
					        );
					        break;
					    case 'T_QTDPROFCREPAR': //QTD de professores em creche tempo parcial
					    	$arrayCols[] = array(
					        	'coluna'	 => 'qtdprofessorcrecheparcial', 
					        	'nomeColuna' => '"QTD de professores em creche tempo parcial"'
					        );
					        break;
					   
					}
				}
				
				
			$sql = "SELECT 
					mun.mundescricao as \"Município\",
					mun.estuf	 as \"UF\",
			";
			
			
			
			if(is_array($arrayCols) && count($arrayCols))
			{
				$i = 0;
				foreach($arrayCols as $k => $v )
				{
					$i++;
					
						if( $i ==  count($arrayCols) )
						{
							
							$sql .= " {$v['coluna']} as {$v['nomeColuna']}
							";
						}
						else 
						{
							$sql .= " {$v['coluna']} as {$v['nomeColuna']},
							";
						}
					
				}
				
			}
			
				$sql .="
										 
					from(
						SELECT distinct
						      t.turdsc, tr.tirdescricao, 
						      t.turid,
						      t.turmes,
						      t.turcadeducacenso,
						      t.turano,
						      ntw.docid,
						      CASE WHEN t.ttuid = 1 THEN 'Exclusivo Creche'
							   WHEN t.ttuid = 2 THEN 'Exclusivo Pré-escola'
							   WHEN t.ttuid = 3 THEN 'Creche e Pré-escola (Misto)'
						      END as tipoatendimento,   
						      t.entcodent,  
						      t.turdtinicio,
						      doc.esdid,
						      esd.esddsc,
						      (select to_char(min(h.htddata), 'YYYY-MM-DD') from workflow.historicodocumento h
						inner join workflow.acaoestadodoc ad on ad.aedid = h.aedid  
						where ad.esdidorigem = 535 and ad.esdiddestino = 536 and h.docid = ntw.docid) as dtanalise,
							case when n.anatipo = 'A' then 'Aprovado'
							  when n.anatipo = 'I' then 'Indeferida'
							  when n.anatipo = 'D' then 'Diligenciada'
							else '<span style=\"color: red;\">Não Analisado</span>' end as tipoanalise,
							n.anaid,
							n.anatipo,
							n.anaparecer,
							t.muncod
						FROM proinfantil.turma t
							inner join proinfantil.tiporede tr ON tr.tirid = t.tirid
							inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = t.turid
							inner join workflow.documento doc on doc.docid = ntw.docid
							inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
							left join proinfantil.analisenovasturmasaprovacao n ON n.turid = t.turid and n.anastatus = 'A'
						    
						WHERE 
							t.turstatus = 'A'
							and doc.esdid != 535
					
						ORDER BY tr.tirdescricao
					) as turma
					
					INNER JOIN territorios.municipio mun ON turma.muncod = mun.muncod
					INNER JOIN
					(
						SELECT
						turid,
							sum(qtdalunospreescolaintegral) as qtdalunospreescolaintegral,
							sum(qtdprofessorpreescolaintegral) as qtdprofessorpreescolaintegral,
					
							sum(	qtdalunospreescolaparcial) as qtdalunospreescolaparcial,
							sum(qtdprofessorpreescolaparcial) as qtdprofessorpreescolaparcial,
					
							sum(qtdalunoscrecheintegral) as qtdalunoscrecheintegral,
							sum(qtdprofessorcrecheintegral) as qtdprofessorcrecheintegral,
					
							sum(	qtdalunoscrecheparcial) as qtdalunoscrecheparcial,
							sum(qtdprofessorcrecheparcial) as qtdprofessorcrecheparcial
					
							
						FROM(
							SELECT 
								(CASE WHEN mds.timid = 7 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.tatid = 1 THEN 'Integral' ELSE 'Parcial' END) AS nome,
								CASE when tur.ttuid = 3 THEN
								(	SELECT 
										vr.vrtvalor FROM proinfantil.valorreferencianovasturmas vr 
									WHERE 
										vr.tatid = mds.tatid  
									AND 
										vr.ttuid = (CASE when mds.timid = 7 THEN 2 else 1 END) 
									AND 
										vr.tirid = tur.tirid AND vr.vrtano = '{$_SESSION['exercicio']}'
								)
								ELSE
									pve.vrtvalor END as vrtvalor,
								n.anatipo,
								tur.turid as turid,
								tur.turdtinicio,
					
								CASE WHEN (CASE WHEN mds.timid = 7 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.tatid = 1 THEN 'Integral' ELSE 'Parcial' END) = 'Pré-Escola Integral' 	THEN  total_alunos  ELSE '0' END as qtdalunospreescolaintegral,
								CASE WHEN (CASE WHEN mds.timid = 7 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.tatid = 1 THEN 'Integral' ELSE 'Parcial' END) = 'Pré-Escola Integral' 	THEN  ntaquantidadeprofessor  ELSE '0' END as qtdprofessorpreescolaintegral,
								
								CASE WHEN (CASE WHEN mds.timid = 7 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.tatid = 1 THEN 'Integral' ELSE 'Parcial' END) = 'Pré-Escola Parcial' 	THEN  total_alunos  ELSE '0' END as qtdalunospreescolaparcial,
								CASE WHEN (CASE WHEN mds.timid = 7 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.tatid = 1 THEN 'Integral' ELSE 'Parcial' END) = 'Pré-Escola Parcial' 	THEN  ntaquantidadeprofessor  ELSE '0' END as qtdprofessorpreescolaparcial,
								
								CASE WHEN (CASE WHEN mds.timid = 7 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.tatid = 1 THEN 'Integral' ELSE 'Parcial' END) = 'Creche Integral'	THEN  total_alunos  ELSE '0' END as qtdalunoscrecheintegral,
								CASE WHEN (CASE WHEN mds.timid = 7 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.tatid = 1 THEN 'Integral' ELSE 'Parcial' END) = 'Creche Integral'	THEN  ntaquantidadeprofessor  ELSE '0' END as qtdprofessorcrecheintegral,
								
								CASE WHEN (CASE WHEN mds.timid = 7 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.tatid = 1 THEN 'Integral' ELSE 'Parcial' END) = 'Creche Parcial'	THEN  total_alunos  ELSE '0' END as qtdalunoscrecheparcial,
								CASE WHEN (CASE WHEN mds.timid = 7 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.tatid = 1 THEN 'Integral' ELSE 'Parcial' END) = 'Creche Parcial'	THEN  ntaquantidadeprofessor  ELSE '0' END as qtdprofessorcrecheparcial,
					
								
								mds.ntaquantidadeprofessor as ntaquantidadeprofessor,
					
								
								( SELECT to_char(min(h.htddata), 'YYYY-MM-DD') FROM workflow.historicodocumento h
							INNER JOIN workflow.acaoestadodoc ad ON ad.aedid = h.aedid  
							WHERE 
								ad.esdidorigem = 535 AND ad.esdiddestino = 536 AND h.docid = ntw.docid) as dataenvio,
								ala.total_alunos
							FROM
								proinfantil.turma tur
							INNER JOIN proinfantil.novasturmasalunoatENDido mds ON mds.turid = tur.turid
							INNER JOIN proinfantil.novasturmasworkflowturma ntw ON ntw.turid = tur.turid
							INNER JOIN (SELECT ntaid, sum(ntaquantidade) AS total_alunos FROM proinfantil.novasturmasalunoatENDido GROUP BY ntaid) as ala ON ala.ntaid = mds.ntaid
							LEFT JOIN proinfantil.valorreferencianovasturmas pve ON pve.tatid = mds.tatid AND pve.tirid = tur.tirid AND pve.ttuid = tur.ttuid AND pve.vrtstatus = 'A' AND pve.vrtano = '{$_SESSION['exercicio']}'
							LEFT JOIN proinfantil.analisenovasturmasaprovacao n ON n.turid = tur.turid AND n.anastatus = 'A'
							WHERE
								
								tur.turstatus = 'A'
							
							ORDER BY
							mds.timid DESC, 
							mds.tatid
						) as dadost
						group by turid 
						order by turid
					) as dturm on dturm.turid = turma.turid
				" ;
				
				
				if( $situacao )
				{
					$sql.= "AND
						turma.esdid  in ( {$situacao} )
						";	
				}
				
				if( $strMunCod )
				{
					$sql.= "AND
						mun.muncod in ( {$strMunCod} )
						
						";	 
				}
				if( $strUf )
				{
					$sql.= "AND
						mun.estuf in ( {$strUf} )
						
						";	 
				}
			
				return $sql;
				
			}
			else 
			{
				return false;
			}
		}//Municipal
		else 
		{
			if(is_array($arrColunas) && ( count($arrColunas) > 0 ))
			{
				$situacao = $_POST['situacao_mun'];
				foreach( $arrColunas as $k => $col )
				{
					switch ($col) {
					    case 'MUN_UF': // UF
					        $arrayCols[] = array(
					        	'coluna'	 => 'mun.estuf', 
					        	'nomeColuna' => '"UF"'
					        );
					        break;
					    case 'MUN_MUN': //Municípios 
					    	$arrayCols[] = array(
					        	'coluna'	 => 'mun.mundescricao', 
					        	'nomeColuna' => '"Municípios"'
					        );
					        break;
					    case 'MUN_MESCAD'://Meses cadastrado 
					    	$arrayCols[] = array(
					        	'coluna'	 => "case 
													when (turm.ntmmmes ) = 1 then
														'Janeiro' 
													 when (turm.ntmmmes) = 2 then
														'Fevereiro'
													when (turm.ntmmmes) = 3 then
														'Março' 
													when (turm.ntmmmes) = 4 then
														'Abril' 
													when (turm.ntmmmes) = 5 then
														'Maio' 
													when (turm.ntmmmes) = 6 then
														'Junho'
													when (turm.ntmmmes) = 7 then
														'Julho'
													when (turm.ntmmmes) = 8 then
														'Agosto'
													when (turm.ntmmmes) = 9 then
														'Setembro'
													when (turm.ntmmmes) = 10 then
														'Outubro' 
													when (turm.ntmmmes) = 11 then
														'Novembro'
													when (turm.ntmmmes) = 12 then
														'Dezembro' 
												end
												", 
					        	'nomeColuna' => '"Meses cadastrado"'
					        );
					        break;
	
					    case 'MUN_QTDTURMCREPUB': //QTD de turmas da creche pública
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdturmacrechepublica - ntmmmes.ntmmqtdturmacrechepublica', 
					        	'nomeColuna' => '"QTD de turmas da creche pública"'
					        );
					        break;
						         
					    case 'MUN_QTDTURMCRECONV'://QTD de turmas da creche conveniada
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdturmacrecheconveniada - ntmmmes.ntmmqtdturmacrecheconveniada', 
					        	'nomeColuna' => '"QTD de turmas da creche conveniada"'
					        );
					        break;
					    case 'MUN_QTDTURMPREPUB': //QTD de turmas da pré-escola pública
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdturmapreescolapublica - ntmmmes.ntmmqtdturmapreescolapublica', 
					        	'nomeColuna' => '"QTD de turmas da pré-escola pública"'
					        );
					        break;
					    case 'MUN_QTDTURMPRECONV': //QTD de turmas da pré-escola conveniadas
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdturmapreescolaconveniada - ntmmmes.ntmmqtdturmapreescolaconveniada', 
					        	'nomeColuna' => '"QTD de turmas da pré-escola conveniadas"'
					        );
					        break;
					     
					    case 'MUN_QTDTURMUNIPUB': //QTD de turmas da Unificada pública
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdturmaunificadapublica - ntmmmes.ntmmqtdturmaunificadapublica', 
					        	'nomeColuna' => '"QTD de turmas da Unificada pública"'
					        );
					        break;
						case 'MUN_QTDTURMUNICONV': //QTD de turmas da Unificada conveniadas
					        $arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdturmaunificadaconveniada - ntmmmes.ntmmqtdturmaunificadaconveniada', 
					        	'nomeColuna' => '"QTD de turmas da Unificada conveniadas"'
					        );
					        break;
					    case 'MUN_TOTAMPTURM': //Total ampliado de Turmas 
					    	$arrayCols[] = array(
					        	'coluna'	 => "( turm.ntmmqtdturmacrechepublica - ntmmmes.ntmmqtdturmacrechepublica )
													 + (turm.ntmmqtdturmacrecheconveniada - ntmmmes.ntmmqtdturmacrecheconveniada)
													 + (turm.ntmmqtdturmapreescolapublica - ntmmmes.ntmmqtdturmapreescolapublica)
													 + (turm.ntmmqtdturmapreescolaconveniada - ntmmmes.ntmmqtdturmapreescolaconveniada) 
													 + (turm.ntmmqtdturmaunificadapublica - ntmmmes.ntmmqtdturmaunificadapublica)
													 + (turm.ntmmqtdturmaunificadaconveniada - ntmmmes.ntmmqtdturmaunificadaconveniada)", 
					        	'nomeColuna' => '"Total ampliado de Turmas"'
					        );
					        break;
					        
					    case 'MUN_QTDMATCREPUBINT': //QTD de matrículas da creche integral pública
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdmatriculacrecheintegralpublica - ntmmmes.ntmmqtdmatriculacrecheintegralpublica2', 
					        	'nomeColuna' => '"QTD de matrículas da creche integral pública"'
					        );
					        break;
					    case 'MUN_QTDMATCRECONVINT': //QTD de matrículas da creche integral conveniada
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdmatriculacrecheintegralconveniada - ntmmmes.ntmmqtdmatriculacrecheintegralconveniada2', 
					        	'nomeColuna' => '"QTD de matrículas da creche integral conveniada"'
					        );
					        break;
					    case 'MUN_QTDMATPREPUBINT': //QTD de matrículas da pré-escola integral pública
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdmatriculapreescolaintegralpublica  - ntmmmes.ntmmqtdmatriculapreescolaintegralpublica2', 
					        	'nomeColuna' => '"QTD de matrículas da pré-escola integral pública"'
					        );
					        break;
					    case 'MUN_QTDMATPRECONVINT': //QTD de matrículas da pré-escola integral conveniadas
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdmatriculapreescolaintegralconveniada - ntmmmes.ntmmqtdmatriculapreescolaintegralconveniada2', 
					        	'nomeColuna' => '"QTD de matrículas da pré-escola integral conveniadas"'
					        );
					        break;
					    case 'MUN_QTDMATCREPUBPARC': //QTD de matrículas da creche parcial pública
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdmatriculacrecheparcialpublica - ntmmmes.ntmmqtdmatriculacrecheparcialpublica2', 
					        	'nomeColuna' => '"QTD de matrículas da creche parcial pública"'
					        );
					        break;
					    case 'MUN_QTDMATCRECONVPARC': //QTD de matrículas da creche parcial conveniada
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdmatriculacrecheparcialconveniada - ntmmmes.ntmmqtdmatriculacrecheparcialconveniada2', 
					        	'nomeColuna' => '"QTD de matrículas da creche parcial conveniada"'
					        );
					        break;
					    case 'MUN_QTDMATPREPUBPARC': //QTD de matrículas da pré-escola  parcial pública
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdmatriculapreescolaparcialpublica - ntmmmes.ntmmqtdmatriculapreescolaparcialpublica2', 
					        	'nomeColuna' => '"QTD de matrículas da pré-escola parcial pública"'
					        );
					        break;
					    case 'MUN_QTDMATPRECONVPARC': //QTD de matrículas da pré-escola parcial conveniadas
					    	$arrayCols[] = array(
					        	'coluna'	 => 'turm.ntmmqtdmatriculapreescolaparcialconveniada - ntmmmes.ntmmqtdmatriculapreescolaparcialconveniada2', 
					        	'nomeColuna' => '"QTD de matrículas da pré-escola parcial conveniadas"'
					        );
					        break;
					    case 'MUN_AMPMAT': //Total ampliado de Matrículas 
					    	$arrayCols[] = array(
					        	'coluna'	 => "	(turm.ntmmqtdmatriculacrecheintegralpublica - ntmmmes.ntmmqtdmatriculacrecheintegralpublica2) +
													(turm.ntmmqtdmatriculacrecheintegralconveniada - ntmmmes.ntmmqtdmatriculacrecheintegralconveniada2) +
													(turm.ntmmqtdmatriculacrecheparcialpublica - ntmmmes.ntmmqtdmatriculacrecheparcialpublica2) +
													(turm.ntmmqtdmatriculacrecheparcialconveniada - ntmmmes.ntmmqtdmatriculacrecheparcialconveniada2) +
													(turm.ntmmqtdmatriculapreescolaintegralpublica  - ntmmmes.ntmmqtdmatriculapreescolaintegralpublica2) +
													(turm.ntmmqtdmatriculapreescolaintegralconveniada - ntmmmes.ntmmqtdmatriculapreescolaintegralconveniada2) +
													(turm.ntmmqtdmatriculapreescolaparcialpublica - ntmmmes.ntmmqtdmatriculapreescolaparcialpublica2) +
													(turm.ntmmqtdmatriculapreescolaparcialconveniada - ntmmmes.ntmmqtdmatriculapreescolaparcialconveniada2) ", 
					        	'nomeColuna' => '"Total ampliado de Matrículas"'
					        );
					        break;
					   
					}
				}
			
			}
			else 
			{
				return false;
			}
			
			
			$sql = " SELECT 
					distinct
					 mun.mundescricao as \"Município\",
					 mun.estuf as \"UF\",	
			";
			
			
			
			if(is_array($arrayCols) && count($arrayCols))
			{
				$i = 0;
				foreach($arrayCols as $k => $v )
				{
					$i++;
					
						if( $i ==  count($arrayCols) )
						{
							
							$sql .= " {$v['coluna']} as {$v['nomeColuna']}
							";
						}
						else 
						{
							$sql .= " {$v['coluna']} as {$v['nomeColuna']},
							";
						}
					
				}
				
			}
			
			$sql .="
				FROM
						
					territorios.municipio mun
				
				inner join (SELECT ntmmid,
						muncod, ntmmstatus, ntmmano, ntmmmes, ntmmqtdmatriculacrecheparcialpublica, ntmmqtdmatriculacrecheintegralpublica,
					ntmmqtdmatriculapreescolaparcialpublica, ntmmqtdmatriculapreescolaintegralpublica, ntmmqtdmatriculacrecheparcialconveniada,
					ntmmqtdmatriculacrecheintegralconveniada, ntmmqtdmatriculapreescolaparcialconveniada, ntmmqtdmatriculapreescolaintegralconveniada,
					ntmmqtdturmacrechepublica, ntmmqtdturmapreescolapublica, ntmmqtdturmaunificadapublica, ntmmqtdturmacrecheconveniada,
					ntmmqtdturmapreescolaconveniada, ntmmqtdturmaunificadaconveniada
				FROM proinfantil.novasturmasdadosmunicipiospormes ntm WHERE ntmmano = '{$_SESSION['exercicio']}' and ntmmstatus = 'A'  and ntmmstatus = 'A'
				order by ntmmmes desc) as turm on turm.muncod = mun.muncod
				
				INNER JOIN(
					 select 
						--creche
						ntmm2.muncod as muncod,
						case when max(ntmm2.ntmmqtdmatriculacrecheintegralpublica) = 0 then max(ntmm2.ntcqtdalunocrecheintegralpublica) else max(ntmm2.ntmmqtdmatriculacrecheintegralpublica) end as ntmmqtdmatriculacrecheintegralpublica2,
						case when max(ntmm2.ntmmqtdmatriculacrecheparcialpublica) = 0 then max(ntmm2.ntcqtdalunocrecheparcialpublica) else max(ntmm2.ntmmqtdmatriculacrecheparcialpublica) end as ntmmqtdmatriculacrecheparcialpublica2,
						case when max(ntmm2.ntmmqtdmatriculacrecheintegralconveniada) = 0 then max(ntmm2.ntcqtdalunocrecheintegralconveniada) else max(ntmm2.ntmmqtdmatriculacrecheintegralconveniada) end as ntmmqtdmatriculacrecheintegralconveniada2,
						case when max(ntmm2.ntmmqtdmatriculacrecheparcialconveniada) = 0 then max(ntmm2.ntcqtdalunocrecheparcialconveniada) else max(ntmm2.ntmmqtdmatriculacrecheparcialconveniada) end as ntmmqtdmatriculacrecheparcialconveniada2,
						--preescola
						case when max(ntmm2.ntmmqtdmatriculapreescolaintegralpublica) = 0 then max(ntmm2.ntcqtdalunopreescolaintegralpublica) else max(ntmm2.ntmmqtdmatriculapreescolaintegralpublica) end as ntmmqtdmatriculapreescolaintegralpublica2,
						case when max(ntmm2.ntmmqtdmatriculapreescolaparcialpublica) = 0 then max(ntmm2.ntcqtdalunopreescolaparcialpublica) else max(ntmm2.ntmmqtdmatriculapreescolaparcialpublica) end as ntmmqtdmatriculapreescolaparcialpublica2,
						case when max(ntmm2.ntmmqtdmatriculapreescolaintegralconveniada) = 0 then max(ntmm2.ntcqtdalunopreescolaintegralconveniada) else max(ntmm2.ntmmqtdmatriculapreescolaintegralconveniada) end as ntmmqtdmatriculapreescolaintegralconveniada2,
						case when max(ntmm2.ntmmqtdmatriculapreescolaparcialconveniada) = 0 then max(ntmm2.ntcqtdalunopreescolaparcialconveniada) else max(ntmm2.ntmmqtdmatriculapreescolaparcialconveniada) end as ntmmqtdmatriculapreescolaparcialconveniada2,
						case when max(ntmmqtdturmacrechepublica) = 0 then max(ntcqtdturmacrechepublica) else max(ntmmqtdturmacrechepublica) end as ntmmqtdturmacrechepublica,
						case when max(ntmmqtdturmapreescolapublica) = 0 then max(ntcqtdturmapreescolapublica) else max(ntmmqtdturmapreescolapublica) end as ntmmqtdturmapreescolapublica,
						case when max(ntmmqtdturmaunificadapublica) = 0 then max(ntcqtdturmaunificadapublica) else max(ntmmqtdturmaunificadapublica) end as ntmmqtdturmaunificadapublica,
						
						case when max(ntmmqtdturmacrecheconveniada) = 0 then max(ntcqtdturmacrecheconveniada) else max(ntmmqtdturmacrecheconveniada) end as ntmmqtdturmacrecheconveniada,
						case when max(ntmmqtdturmapreescolaconveniada) = 0 then max(ntcqtdturmapreescolaconveniada) else max(ntmmqtdturmapreescolaconveniada) end as ntmmqtdturmapreescolaconveniada,
						case when max(ntmmqtdturmaunificadaconveniada) = 0 then max(ntcqtdturmaunificadaconveniada) else max(ntmmqtdturmaunificadaconveniada) end as ntmmqtdturmaunificadaconveniada,
						max(ntmm2.ntmmmes) as ntmmmes2
					from(
						SELECT
							dc.muncod as muncod,
							--creche censo
							COALESCE(SUM(dc.ntcqtdalunocrecheintegralpublica),0) as ntcqtdalunocrecheintegralpublica,
							COALESCE(SUM(dc.ntcqtdalunocrecheparcialpublica),0) as ntcqtdalunocrecheparcialpublica,
							COALESCE(SUM(dc.ntcqtdalunocrecheintegralconveniada),0) as ntcqtdalunocrecheintegralconveniada,
							COALESCE(SUM(dc.ntcqtdalunocrecheparcialconveniada),0) as ntcqtdalunocrecheparcialconveniada,
							--preescola censo
							COALESCE(SUM(dc.ntcqtdalunopreescolaintegralpublica),0) as ntcqtdalunopreescolaintegralpublica,
							COALESCE(SUM(dc.ntcqtdalunopreescolaparcialpublica),0) as ntcqtdalunopreescolaparcialpublica,
							COALESCE(SUM(dc.ntcqtdalunopreescolaintegralconveniada),0) as ntcqtdalunopreescolaintegralconveniada,
							COALESCE(SUM(dc.ntcqtdalunopreescolaparcialconveniada),0) as ntcqtdalunopreescolaparcialconveniada,
							dm.ntmmid,
							--creche matricula
							COALESCE(dm.ntmmqtdmatriculacrecheintegralpublica, 0) as ntmmqtdmatriculacrecheintegralpublica,
							COALESCE(dm.ntmmqtdmatriculacrecheparcialpublica, 0) as ntmmqtdmatriculacrecheparcialpublica,
							COALESCE(dm.ntmmqtdmatriculacrecheintegralconveniada, 0) as ntmmqtdmatriculacrecheintegralconveniada,
							COALESCE(dm.ntmmqtdmatriculacrecheparcialconveniada, 0) as ntmmqtdmatriculacrecheparcialconveniada,
							--preescola matricula
							COALESCE(dm.ntmmqtdmatriculapreescolaintegralpublica, 0) as ntmmqtdmatriculapreescolaintegralpublica,
							COALESCE(dm.ntmmqtdmatriculapreescolaparcialpublica, 0) as ntmmqtdmatriculapreescolaparcialpublica,
							COALESCE(dm.ntmmqtdmatriculapreescolaintegralconveniada, 0) as ntmmqtdmatriculapreescolaintegralconveniada,
							COALESCE(dm.ntmmqtdmatriculapreescolaparcialconveniada, 0) as ntmmqtdmatriculapreescolaparcialconveniada,
							 COALESCE(SUM(dc.ntcqtdturmacrechepublica),0) as ntcqtdturmacrechepublica,
							COALESCE(SUM(dc.ntcqtdturmapreescolapublica),0) as ntcqtdturmapreescolapublica,
							COALESCE(SUM(dc.ntcqtdturmaunificadapublica),0) as ntcqtdturmaunificadapublica,
							COALESCE(SUM(dc.ntcqtdturmacrecheconveniada),0) as ntcqtdturmacrecheconveniada,
							COALESCE(SUM(dc.ntcqtdturmapreescolaconveniada),0) as ntcqtdturmapreescolaconveniada,
							COALESCE(SUM(dc.ntcqtdturmaunificadaconveniada),0) as ntcqtdturmaunificadaconveniada,
							dm.ntmmid,
							dm.ntmmmes,
							COALESCE(dm.ntmmqtdturmacrechepublica, 0) as ntmmqtdturmacrechepublica,
							COALESCE(dm.ntmmqtdturmapreescolapublica, 0) as ntmmqtdturmapreescolapublica,
							COALESCE(dm.ntmmqtdturmaunificadapublica, 0) as ntmmqtdturmaunificadapublica,
							COALESCE(dm.ntmmqtdturmacrecheconveniada, 0) as ntmmqtdturmacrecheconveniada,
							COALESCE(dm.ntmmqtdturmapreescolaconveniada, 0) as ntmmqtdturmapreescolaconveniada,
							COALESCE(dm.ntmmqtdturmaunificadaconveniada, 0) as ntmmqtdturmaunificadaconveniada,
							dc.ntcanocenso					
						FROM proinfantil.novasturmasdadoscenso dc
							LEFT JOIN proinfantil.novasturmasdadosmunicipiospormes dm on dm.muncod = dc.muncod and dm.ntmmano = '{$_SESSION['exercicio']}' and dm.ntmmmes = 0 and dm.ntmmstatus = 'A'		
					    WHERE -- dc.muncod = '1200203'
						-- AND 
						dc.ntcanocenso = '{$anoCenso}'
						and dc.ntcstatus = 'A'
						GROUP BY
							dc.muncod,
							dm.ntmmid,
							dc.ntcanocenso,
							dm.ntmmmes,
							dm.ntmmqtdmatriculacrecheintegralpublica,
							dm.ntmmqtdmatriculacrecheparcialpublica,
							dm.ntmmqtdmatriculacrecheintegralconveniada,
							dm.ntmmqtdmatriculacrecheparcialconveniada,
							dm.ntmmqtdmatriculapreescolaintegralpublica,
							dm.ntmmqtdmatriculapreescolaparcialpublica,
							dm.ntmmqtdmatriculapreescolaintegralconveniada,
							dm.ntmmqtdmatriculapreescolaparcialconveniada,
							 dm.ntmmid,
							dc.ntcanocenso,
							dm.ntmmqtdturmacrechepublica,
							dm.ntmmqtdturmapreescolapublica,
							dm.ntmmqtdturmaunificadapublica,
							dm.ntmmqtdturmacrecheconveniada,
							dm.ntmmqtdturmapreescolaconveniada,
							dm.ntmmqtdturmaunificadaconveniada
					) as ntmm2 
					group by ntmm2.muncod 
					) ntmmmes on ntmmmes.muncod = turm.muncod			
				
			" ;
			
			
			if( $strMunCod )
			{
				$sql.= "AND
					mun.muncod in ( {$strMunCod} )
					
					";	 
			}
			if( $strUf )
			{
				$sql.= "AND
					mun.estuf in ( {$strUf} )
					
					";	 
			}
		
			return $sql;
			
		}
		
		return false;
		
	}
	
	function retornaDados()
	{
		$sql   = $this->monta_sql();
		if($sql)
		{
			
			return $this->dados = $this->db->carregar( $sql );	
		}
		else
		{
			return array();
		}
		
		
	}
}




?>
<html>
	<head>
		
<?php 
	if( ! $isXls )
{
?>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php 
}
?>
		
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
// Faz requisição para o método retornaDados que irá retornar o array com os dados do relatório
$dados =  $obRelatorio->retornaDados();

// Caso exista dados ele  monta o relatório, senão exibe mensagem de que não existem registros
if( is_array($dados) && count($dados))
{
	// Caso seja uma exportação para excel
	if($isXls)
	{
		$file_name="relatorioNovasTurmas.xls";
		header("Pragma: public");
		header("Expires: 0");
		header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
		header("Content-Type: application/force-download");
		header("Content-Type: application/octet-stream");
		header("Content-Type: application/download");
		header("Content-Disposition: attachment;filename=".$file_name);
		header("Content-Transfer-Encoding: binary ");
	}
	
	// Monta o cabeçalho com o Brasao
	$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
	$cabecalhoBrasao .= "<tr>" .
					    "<td colspan=\"100\">" .			
						monta_cabecalho_relatorio('100') .
					    "</td>" .
				        "</tr>
				        </table>";
	
					
	// Caso não seja excel monta cabeçalho e título						
	if( ! $isXls )
	{
		echo $cabecalhoBrasao;
		monta_titulo('Relatório Novas Turmas','');
	}
	// Monta tabela que irá exibir os dados
	echo '<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">';
	// Cabeçalho para o arquivo excel
	if( $isXls )
	{
		echo ' <tr style="background-color: #dcdcdc;"> <td colspan="' . count($dados[0]) . '" > Relatório Dados da Unidade </td></tr>';
	}
	
	// Começa linha com os nomes das colunas
	echo '	<tr >' ;
	
	foreach($dados[0] as $colKey => $colVal )
	{
		echo "		<td style=\"background-color: #e9e9e9;\" align=\'center\'  > <b> {$colKey} </b> </td>";
	}
	echo '</tr>';
	
	// Lista as informações do relatório
	foreach($dados as $linhaKey => $linhaVal )
	{
		echo '<tr>';
		foreach( $linhaVal as $colKey => $colVal )
		{
			echo " <td align=\'center\' width=\'50%\' > <b> {$colVal} </b> </td>";	
		}
		
	}
	// Finaliza o relatório
	echo '</table>';
	
}
else 
{
	// Exibe mensagem de que não existem registros.
	die( '<table cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">'.
										'<TR style="background:#FFF; color:red;">'.
											'<TD colspan="10" align="center">'.
												'Não foram encontrados registros.'.
											'</TD>'.
										'</TR>'.
								   '</table>');
}
 
if($isXls)
{
	// Finaliza processo
	die('<script>window.close();</script>');
}

?>

</body>