<?
set_time_limit(0);

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

//xx($_POST);

$sql   = monta_sql();
$dados = $db->carregar($sql);

//ver($dados,d);
$agrup = monta_agp();
$col   = monta_coluna();

//dbg($agrup);
//dbg($col,1);
//dbg($sql,1);
//ver($col ,d)
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 

//ver($agrup,d);
$r->setColuna($col);
$r->setBrasao($true ? true : false);
$r->setTotNivel(true);
$r->setEspandir($_POST['expandir']);
//$r->setBrasao(true);
if( $_POST['req'] == 2 ){
	ob_clean();
	$nomeDoArquivoXls="Relatório_Programa_".$prgid."_".date('d-m-Y_H_i');
	echo $r->getRelatorioXls();
}else{
	echo $r->getRelatorio();
}


function monta_sql(){
	
	extract($_POST);
	

	if ($prgid){
		$where[] = " pa.prgid = ".$prgid;
	}
	if ($esfera == '2'){ //municipio
		$where[] = " i.itrid = '".$esfera."'";
		if($muncod[0]) $where[] = " i.muncod in ('".implode( "','", $muncod )."')";
		if($uf[0]) $where[] = " i.mun_estuf in ('".implode( "','", $uf )."')";
		$campoEstado = "i.mun_estuf as estado";
		$campoEstadoOrder = "i.mun_estuf";
	}	
	else if ($esfera == '1'){ //estado
		$where[] = " i.itrid = '".$esfera."'";
		if($uf[0]) $where[] = " i.estuf in ('".implode( "','", $uf )."')";
		$campoEstado = "i.estuf as estado";
		$campoEstadoOrder = "i.estuf";
	}
	else{
		//$where[] = " i.itrid = '".$esfera."'";
		if($uf[0]) $where[] = " ( i.estuf in ('".implode( "','", $uf )."') OR i.mun_estuf in ('".implode( "','", $uf )."') )";
		$campoEstado = "(CASE WHEN i.itrid = 2 THEN
						i.mun_estuf
					      ELSE
						i.estuf
					END) as estado";
		$campoEstadoOrder = "i.estuf";
	}
	

	
	// formação
	$leftInnerFormacao = "LEFT";
	if( $formacaof[0] ){
		$leftInnerFormacao = "INNER";
		$where[] = " tf.tfoid " . (!$formacaof_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $formacaof ) . "')";
	}
	
	// vinculo empregaticio
	$leftInnerVinculoEmpreg = "LEFT";
	if( $tvpidf[0] ){
		$leftInnerVinculoEmpreg = "INNER";
		$where[] = " pfv.tvpid " . (!$tvpidf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tvpidf ) . "')";
	}
		
	// curso
	if( !in_array("coordenador",$_POST['agrupador']) && !in_array("professor",$_POST['agrupador']) ){
		if( $cursof[0] ){
			$where[] = " pcu.pfcid " . (!$cursof_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $cursof ) . "')";
		}	
	}	

	// escolas
	$leftInnerEscola = "LEFT";
	if( $escolaf[0] ){
		$leftInnerEscola = "INNER";
		$where[] = " ent2.entid " . (!$escolaf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $escolaf ) . "')";
	}

	
	
	//$leftInnerMunicipio = "LEFT";
	// Arrumando para aparecer municípios quando for esfera por estado.
	$innerMun = "LEFT join public.municipio m on m.muncod = i.muncod";
	if( in_array("municipio",$_POST['agrupador'])  ){
		if( $esfera == 1 ){
			$innerMun = "INNER join entidade.endereco ende		on ende.entid = pc.entid
						 INNER join public.municipio m    		on m.muncod = ende.muncod";
		} else {
			$innerMun = "inner join public.municipio m on m.muncod = i.muncod";
		}	
	}
	
	
	$leftInnerCursoAluno = "LEFT";
	if( in_array("curso",$_POST['agrupador']) || in_array("aluno",$_POST['agrupador']) ){
		$leftInnerCursoAluno = "INNER";	
		
//												"orgao",
//												"",
//												"",
//												"",
//												"",
		$sql = "select 
					pc.pcunome as aluno,
                                        pfv.tvpdsc || ' '  as vinculo,
                                        '(' || pc.pcudddnumtelefone || ') ' || pc.pcunumtelefone as telefone,
					pc.pcucpf || ' ' as cpf,
					pff.pffdescricao || ' ' as funcao_atual,
					pc.pcuorgao || ' ' as orgao,
					'Fase ' || pc.pcufasecurso AS fase,
                                        CASE WHEN pc.pcuexerciciosecretariaeducacao = 't' THEN 'Sim' ELSE 'Não' END AS exercicio,
					CASE WHEN pc.pcucargoefetivo = 't' THEN 'Sim' ELSE 'Não' END AS cargo_efetivo,
					pc.pcuemail || ' ' as email,
					tf.tfodsc as formacao,
					pcu.pfcdescricao as curso,
					ent2.entnome as escola,
					m.mundsc as municipio,
					'Brasil' as pais,
					'1' as qtde,
					$campoEstado		
				from par.pfadesao pa
				inner join par.pfadesaoprograma ap on ap.pfaid = pa.pfaid
				inner join par.instrumentounidade i on i.inuid = ap.inuid
				--$leftInnerMunicipio join public.municipio m on m.muncod = i.muncod
				$leftInnerCursoAluno join par.pfcursista pc on pc.adpid = ap.adpid
				$leftInnerCursoAluno join par.pfcurso pcu on pcu.pfcid = pc.pfcid
				$leftInnerFormacao join public.tipoformacao tf on tf.tfoid = pc.tfoid
				$leftInnerVinculoEmpreg join public.tipovinculoprofissional pfv ON pfv.tvpid = pc.tvpid
				$leftInnerEscola join entidade.entidade ent2 on ent2.entid = pc.entid
                                left join par.pffuncao pff on pff.pffid = pc.pffid
				$innerMun
				where ap.adpresposta = 'S'
				" . (is_array($where) ? " AND ".implode(' AND ', $where) : '') . "
				order by $campoEstadoOrder, m.mundsc, pcu.pfcdescricao, pc.pcunome
				";	
//ver($sql,d);
		
	}
	
	
	$leftInnerCoordenador = "LEFT";
	if( in_array("coordenador",$_POST['agrupador'])  ){
		$leftInnerCoordenador = "INNER";	

		$sql = "select 
					e.entnome as coordenador,
					e.entnumcpfcnpj || ' ' as cpf,
					e.entemail || ' ' as email,
					tf.tfodsc as formacao,
					ent2.entnome as escola,
					m.mundsc as municipio,
					'Brasil' as pais,
					'1' as qtde,
					$campoEstado		
				from par.pfadesao pa
				inner join par.pfadesaoprograma ap on ap.pfaid = pa.pfaid
				inner join par.instrumentounidade i on i.inuid = ap.inuid
				$leftInnerCoordenador join par.pfentidade pe on pe.adpid = ap.adpid
				$leftInnerCoordenador join entidade.entidade e on e.entid = pe.entid
				$leftInnerCoordenador join entidade.funcaoentidade fe on fe.entid = e.entid and fe.funid = 87 -- coordenador
				$leftInnerEscola JOIN entidade.funcao 	fun ON fun.funid = fe.funid
				$leftInnerEscola JOIN entidade.funentassoc fea ON fea.fueid = fe.fueid
				$leftInnerEscola JOIN entidade.entidade  ent2 ON ent2.entid = fea.entid
				$leftInnerFormacao join public.tipoformacao tf on tf.tfoid = pe.tfoid
				$leftInnerVinculoEmpreg join public.tipovinculoprofissional	pfv ON pfv.tvpid = pe.tvpid
				$innerMun
				where ap.adpresposta = 'S'
				" . (is_array($where) ? " AND ".implode(' AND ', $where) : '') . "
				order by $campoEstadoOrder, m.mundsc, e.entnome
				";			
	}
	
	$leftInnerProfessor = "LEFT";
	if( in_array("professor",$_POST['agrupador'])  ){
		$leftInnerProfessor = "INNER";	
		
		$sql = "select 
				  	e2.entnome as professor,
				  	e2.entnumcpfcnpj || ' ' as cpf,
					e2.entemail || ' ' as email,
					tf.tfodsc as formacao,
					ent2.entnome as escola,
					m.mundsc as municipio,
					'Brasil' as pais,
					'1' as qtde,
					$campoEstado		
				from par.pfadesao pa
				inner join par.pfadesaoprograma ap on ap.pfaid = pa.pfaid
				inner join par.instrumentounidade i on i.inuid = ap.inuid
				$leftInnerProfessor join par.pfentidade pe2 on pe2.adpid = ap.adpid
				$leftInnerProfessor join entidade.entidade e2 on e2.entid = pe2.entid
				$leftInnerProfessor join entidade.funcaoentidade fe2 on fe2.entid = e2.entid and fe2.funid = 88 -- professor
				$leftInnerEscola JOIN entidade.funcao 	fun ON fun.funid = fe2.funid
				$leftInnerEscola JOIN entidade.funentassoc fea ON fea.fueid = fe2.fueid
				$leftInnerEscola JOIN entidade.entidade  ent2 ON ent2.entid = fea.entid
				$leftInnerFormacao join public.tipoformacao tf on tf.tfoid = pe2.tfoid
				$leftInnerVinculoEmpreg join public.tipovinculoprofissional	pfv ON pfv.tvpid = pe2.tvpid
				$innerMun
				where ap.adpresposta = 'S'
				" . (is_array($where) ? " AND ".implode(' AND ', $where) : '') . "
				order by $campoEstadoOrder, m.mundsc, e2.entnome
				";			
	}
	
	//somente para o programa prgid=183 (agua na escola)
	if($prgid == 183){
		$sql = "select 
					ent2.entnome as escola,
					m.mundsc as municipio,
					'Brasil' as pais,
					'1' as qtde,
					$campoEstado		
				from par.pfadesao pa
				inner join par.pfadesaoprograma ap on ap.pfaid = pa.pfaid
				inner join par.instrumentounidade i on i.inuid = ap.inuid
				--inner join (select adpid, max(paeid) as paeid from par.pffotosaguaescola group by adpid) pf on pf.adpid = ap.adpid
				inner join par.pffotosaguaescola pf on pf.adpid = ap.adpid
				inner join par.pfaguaescola pe on pe.paeid = pf.paeid
				$leftInnerEscola join entidade.entidade ent2 on ent2.entcodent = pe.entcodent
				$innerMun
				where ap.adpresposta = 'S'
				" . (is_array($where) ? " AND ".implode(' AND ', $where) : '') . "
				order by $campoEstadoOrder, m.mundsc, ent2.entnome
				";
				//dbg($sql,1);
	}
	
	if(!$sql){
		$sql = "select 
					pc.pcunome as aluno,
					--e.entnome as coordenador,		
				  	--e2.entnome as professor,
					pcu.pfcdescricao as curso,
					ent2.entnome as escola,
					tf.tfodsc as formacao,
					m.mundsc as municipio,
					'Brasil' as pais,
					'1' as qtde,
					$campoEstado		
				from par.pfadesao pa
				inner join par.pfadesaoprograma ap on ap.pfaid = pa.pfaid
				inner join par.instrumentounidade i on i.inuid = ap.inuid
				$leftInnerCursoAluno join par.pfcursista pc on pc.adpid = ap.adpid
				$leftInnerCursoAluno join par.pfcurso pcu on pcu.pfcid = pc.pfcid
				--$leftInnerCoordenador join par.pfentidade pe on pe.adpid = ap.adpid
				--$leftInnerCoordenador join entidade.entidade e on e.entid = pe.entid
				--$leftInnerCoordenador join entidade.funcaoentidade fe on fe.entid = e.entid and fe.funid = 87 -- coordenador
				--$leftInnerProfessor join par.pfentidade pe2 on pe2.adpid = ap.adpid
				--$leftInnerProfessor join entidade.entidade e2 on e2.entid = pe2.entid
				--$leftInnerProfessor join entidade.funcaoentidade fe2 on fe2.entid = e2.entid and fe2.funid = 88 -- professor
				$leftInnerFormacao join public.tipoformacao tf on tf.tfoid = pc.tfoid
				$leftInnerVinculoEmpreg join public.tipovinculoprofissional pfv ON pfv.tvpid = pc.tvpid
				$leftInnerEscola join entidade.entidade ent2 on ent2.entid = pc.entid
				$innerMun
				where ap.adpresposta = 'S'
				" . (is_array($where) ? " AND ".implode(' AND ', $where) : '') . "
				order by $campoEstadoOrder, m.mundsc, pcu.pfcdescricao, /*e2.entnome, e.entnome,*/ pc.pcunome
				";
				//dbg($sql,1);
	}			
	//dbg($sql,1);

	return $sql;
}


/**
 * Monta as colunasd de acordo com o agrupador
 * @global type $db
 * @return array
 */
function monta_agp(){
	
	global $db;
	
	$agrupador = $_REQUEST['agrupador'];
	
	/*
	if (!$agrupador){
		$agrupador = array(
							'instituicao',
							'tiposolicitacao',
							'objeto',
							'justificativa',
							'valor'
						 );
	}elseif ( !is_array($agrupador) ){
		$agrupador = explode(",", $agrupador);
	}	
	*/
	
	if( in_array("aluno",$_POST['agrupador']) || in_array("professor",$_POST['agrupador']) || in_array("coordenador",$_POST['agrupador']) ){
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"nome",
												"vinculo",
												"telefone",
												"orgao",
												"funcao_atual",
												"fase",
												"exercicio",
												"cargo_efetivo",
												"curso",
												"escola",
												"cpf",
												"formacao",
												"email",
												"qtde"
											  )	  
					);
	}
	else{
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"qtde"
											  )	  
					);
	}	
	
	if($agrupador){
		
			
		foreach ($agrupador as $val): 
			switch ($val) {
				case 'pais':
					array_push($agp['agrupador'], array(
														"campo" => "pais",
												  		"label" => "País")										
										   				);				
			    	continue;
			        break;
				case 'estado':
					array_push($agp['agrupador'], array(
														"campo" => "estado",
												  		"label" => "Estado")										
										   				);				
			    	continue;
			        break;				
				case 'municipio':
					array_push($agp['agrupador'], array(
														"campo" => "municipio",
												  		"label" => "Município")										
										   				);				
			    	continue;
			        break;		        		    	
				case 'curso':
					array_push($agp['agrupador'], array(
														"campo" => "curso",
												  		"label" => "Curso")										
										   				);				
					continue;
			        break;
				case 'escola':
					array_push($agp['agrupador'], array(
														"campo" => "escola",
												  		"label" => "Escola")										
										   				);				
					continue;
			        break;
			    case 'aluno':
					array_push($agp['agrupador'], array(
														"campo" => "aluno",
												  		"label" => "Nome do Aluno")										
										   				);
									
					continue;
			        break;
			    case 'professor':
					array_push($agp['agrupador'], array(
														"campo" => "professor",
												  		"label" => "Nome do Professor")										
										   				);				
					continue;
			        break;
			    case 'coordenador':
					array_push($agp['agrupador'], array(
														"campo" => "coordenador",
												  		"label" => "Nome do Coordenador")										
										   				);				
					continue;
			        break;    
			   	        	
			}
		endforeach;
		
	}
	
	return $agp;
}
//CPF	Nome	Email	Telefone	Formação	Vínculo	Função atual	Órgão	Cargo Efetivo	Exercício	Fase
/**
 * Monta as colunas de acordo com o formulario
 * @return array
 */
function monta_coluna(){
	
	
	$agrupador = $_REQUEST['agrupador'];
	$colunas = $_REQUEST['coluna'];
	$coluna = array();
	
	
	if( in_array("aluno",$_POST['agrupador']) || in_array("professor",$_POST['agrupador']) || in_array("coordenador",$_POST['agrupador']) ){
		
		if($colunas){
			
			foreach ($colunas as $val): 
				switch ($val) {
					case 'cpf':
						array_push($coluna, array(
														"campo" => "cpf",
												  		"label" => "CPF")										
										   				);					
						continue;
				        break;
//					case 'nome':
//						array_push($coluna, array(
//														"campo" => "nome",
//												  		"label" => "Nome")										
//										   				);					
//						continue;
//				        break;
					case 'telefone':
						array_push($coluna, array(
														"campo" => "telefone",
												  		"label" => "Telefone")										
										   				);					
						continue;
				        break;
					case 'vinculo':
						array_push($coluna, array(
														"campo" => "vinculo",
												  		"label" => "Vínculo")										
										   				);					
						continue;
				        break;
					case 'funcao_atual':
						array_push($coluna, array(
														"campo" => "funcao_atual",
												  		"label" => "Função atual")										
										   				);					
						continue;
				        break;
					case 'orgao':
						array_push($coluna, array(
														"campo" => "orgao",
												  		"label" => "Órgão")										
										   				);					
						continue;
				        break;
					case 'cargo_efetivo':
						array_push($coluna, array(
														"campo" => "cargo_efetivo",
												  		"label" => "Cargo Efetivo")										
										   				);					
						continue;
				        break;
					case 'exercicio':
						array_push($coluna, array(
														"campo" => "exercicio",
												  		"label" => "Exercício")										
										   				);					
						continue;
				        break;
					case 'fase':
						array_push($coluna, array(
														"campo" => "fase",
												  		"label" => "Fase")										
										   				);					
						continue;
				        break;
				    case 'email':
						array_push($coluna, array(
														"campo" => "email",
												  		"label" => "E-mail")										
										   				);					
						continue;
				        break;
				    case 'formacao':
						array_push($coluna, array(
														"campo" => "formacao",
												  		"label" => "Formação")										
										   				);				
						continue;
				        break;    
				    case 'curso':
						array_push($coluna, array(
														"campo" => "curso",
												  		"label" => "Curso")										
										   				);				
						continue;
				        break;    
				        
				}
			endforeach;
			
		}
	}
	
	
	if($agrupador){
		
			
		foreach ($agrupador as $val): 
			switch ($val) {
				case 'curso':
					$labelQtde = " de Alunos";				
					continue;
			        break;
				case 'aluno':
					$labelQtde = " de Alunos";					
					continue;
			        break;
			    case 'professor':
					$labelQtde = " de Professores";					
					continue;
			        break;
			    case 'coordenador':
					$labelQtde = " de Coordenadores";				
					continue;
			        break;    
			   	        	
			}
		endforeach;
		
		
		
	}
		
	if($_REQUEST['prgid'] == '183'){
		$labelQtde = " de Fotos";	
	}
	
	
	array_push($coluna, array(
							  "campo" => "qtde",
					   		  "label" => "Quantidade" .$labelQtde,
							  "type"  => "numeric"
						)										
			   );
	
   /*
	$coluna    = array(
						array(
							  "campo" => "qtde",
					   		  "label" => "Quantidade" .$labelQtde,
							  "type"  => "numeric"
						)						
					);
	*/	
	/*				
	if($agrupador){
		
			
		foreach ($agrupador as $val): 
			switch ($val) {
				case 'municipio':
					array_push($coluna, array(
														"campo" => "municipio",
												  		"label" => "Município")										
										   				);				
			    	continue;
			        break;		        		    	
				case 'curso':
					array_push($coluna, array(
														"campo" => "curso",
												  		"label" => "Curso")										
										   				);				
					continue;
			        break;
				case 'aluno':
					array_push($coluna, array(
														"campo" => "aluno",
												  		"label" => "Aluno")										
										   				);				
					continue;
			        break;
			    case 'professor':
					array_push($coluna, array(
														"campo" => "professor",
												  		"label" => "Professor")										
										   				);				
					continue;
			        break;
			    case 'coordenador':
					array_push($coluna, array(
														"campo" => "coordenador",
												  		"label" => "Coordenador")										
										   				);				
					continue;
			        break;    
			   	        	
			}
		endforeach;
		
	}
	*/			
					
					
	return $coluna;			  	
}
?>
</body>