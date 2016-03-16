<?php
ini_set("memory_limit", "2048M");
set_time_limit(0);	

if ($_REQUEST['filtrosession']){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header('Content-Type: text/html; charset=iso-8859-1'); 
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
	
	$sql   = monta_sql();
	
	$dados = $db->carregar($sql);
	$agrup = monta_agp();
	$col   = monta_coluna();
	
	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $dados); 
	$r->setColuna($col);
	$r->setBrasao(true);
	$r->setTotNivel(true);
	$r->setEspandir(false);
	$r->setMonstrarTolizadorNivel(true);
	$r->setTotalizador(true);
	$r->setTolizadorLinha(true);

	if($_POST['tipo_relatorio'] == 'xls'){
		ob_clean();
		$nomeDoArquivoXls="relatorio_analitico_".date('d-m-Y_H_i');
		echo $r->getRelatorioXls();
	}elseif($_POST['tipo_relatorio'] == 'visual'){
		echo $r->getRelatorio();	
	}
?>

<?php 
function monta_sql(){
	global $filtroSession;
	
	extract($_POST);
	
	if( $estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){		
		$where[0] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
	}
	
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND ende.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	
	if($memanoreferencia != ''){
		$where[2] = " AND mem.memanoreferencia = $memanoreferencia";
		$where[9] = "WHERE a.meaano = me.memanoreferencia AND me.memanoreferencia = $memanoreferencia";
	}
	
	/*
	if($memanoreferencia == '2012'){
		$where[2] = " AND mem.memanoreferencia = '2012'";
		
		$where[9] = "WHERE a.meaano = me.memanoreferencia AND me.memanoreferencia = '2012'";
		
	}elseif($memanoreferencia == '2011'){
		$where[2] = " AND mem.memanoreferencia = '2011'";
		
		$where[9] = "WHERE a.meaano = me.memanoreferencia AND me.memanoreferencia = '2011'";
		
		
	}elseif($memanoreferencia == '2010'){
		$where[2] = " AND mem.memanoreferencia = '2010'";		
		
		$where[9] = "WHERE a.meaano = me.memanoreferencia AND me.memanoreferencia = '2010'";
		
	}elseif($memanoreferencia == '2009'){
		$where[2] = " AND mem.memanoreferencia = '2009'";	

		$where[9] = "WHERE a.meaano = me.memanoreferencia AND me.memanoreferencia = '2009'";
		
	}elseif($memanoreferencia == '2008'){
		$where[2] = " AND mem.memanoreferencia = '2008'";		
		
		$where[9] = "WHERE a.meaano = me.memanoreferencia AND me.memanoreferencia = '2008'";		
	}
	*/
	
	if( $esfera != ''){
		$where[3] = " AND ent.tpcid IN ($esfera) ";		
	}
	
	if( $macrocampo != ''){
		$where[4] = " AND mtm.mtmid = $macrocampo ";		
	}	
	
	if( $mempagofnde != ''){
		$where[5] = " AND mem.mempagofnde = '$mempagofnde' ";		
	}	

	if( $mamescolaaberta != ''){
		$where[6] = " AND mem.mamescolaaberta = '$mamescolaaberta' ";		
	}	

	if( $membrasilsemmiseria != ''){
		$where[7] = " AND mem.membrasilsemmiseria = '$membrasilsemmiseria' ";		
	}

	if($memclassificacaoescola == 'U'){
		$where[8] = " AND  mem.memclassificacaoescola = 'U' ";		
	}elseif ($memclassificacaoescola == 'R'){
		$where[8] = " AND  mem.memclassificacaoescola = 'R' ";
	}else{
		$where[8] = "";
	}
	
	$sql = "SELECT 
                    me.meaid, me.mtadescricao, me.mtmdescricao,
                    me.tpcid,
	                me.estuf,
    	            me.mundescricao, 
	                me.entcodent,
	                me.entnome,
	                me.endereco,
                	me.entemail,
               		me.telefone, 
                	me.diretor AS diretor,
                	me.cpf_diretor AS cpf_diretor,
                	me.email_diretor AS email_diretor,
                	me.telefone_diretor AS telefone_diretor, 
                	me.celular_diretor AS celular_diretor, 
                	me.coordenador AS coordenador,
                	me.cpf_coordenador AS cpf_coordenador,
                	me.email_coordenador AS email_coordenador,
                	me.telefone_coordenador AS telefone_coordenador,
                	me.celular_coordenador AS celular_coordenador,
                	CASE WHEN me.mempagofnde = 't' THEN 'SIM' ELSE 'NÃO' END as mempagofnde,
                	me.memvlrpago,
                    CASE WHEN me.mamescolaaberta = 't' THEN 'SIM' ELSE 'NÃO' END as mamescolaaberta,
                    CASE WHEN me.membrasilsemmiseria = 't' THEN 'SIM' ELSE 'NÃO' END as membrasilsemmiseria,        	
                	AL.AlunosParticipantes AS alunos_participantes_total,
                	AP.AlunoAtividade AS alunos_participantes_atividade
 			 FROM 
		            (SELECT 
                              mem.memid as memid, mtm.mtmdescricao, mea.meaid, mtadescricao,
                              CASE WHEN ent.tpcid = 1 THEN 'Estadual' ELSE 'Municipal' END as tpcid,
                              ende.estuf,
                              mun.mundescricao, 
                              ent.entcodent, 
                              ent.entnome,
                              ende.endlog || ' - ' ||  ende.endbai || ' - ' || ende.endnum  as endereco,
                              ent.entemail,
                              ent.entnumdddcomercial || ' - ' || ent.entnumcomercial as telefone, 
                              ORGAOFUNCAO.entnome as diretor,
                              ORGAOFUNCAO.entnumcpfcnpj as cpf_diretor,
                              ORGAOFUNCAO.entemail as email_diretor,
                              ORGAOFUNCAO.entnumdddcomercial || ' - ' || ORGAOFUNCAO.entnumcomercial as telefone_diretor, 
                              ORGAOFUNCAO.entnumdddcelular || ' - ' || ORGAOFUNCAO.entnumcelular as celular_diretor, 
                              COORD.entnome as coordenador,
                              COORD.entnumcpfcnpj as cpf_coordenador,
                              COORD.entemail as email_coordenador,
                              COORD.entnumdddcomercial || ' - ' || COORD.entnumcomercial as telefone_coordenador, 
                              COORD.entnumdddcelular || ' - ' || COORD.entnumcelular as celular_coordenador,               
                              CASE WHEN mem.mempagofnde = 't' THEN 'SIM' ELSE 'NÃO' END as mempagofnde,
                              mem.memvlrpago,
                              CASE WHEN mem.mamescolaaberta = 't' THEN 'SIM' ELSE 'NÃO' END as mamescolaaberta,
                              CASE WHEN mem.membrasilsemmiseria = 't' THEN 'SIM' ELSE 'NÃO' END as membrasilsemmiseria
                        FROM pdeescola.memaiseducacao mem
                        LEFT JOIN entidade.entidade ent ON ent.entid = mem.entid
                        LEFT JOIN entidade.funentassoc f ON ent.entid = f.entid
                        LEFT JOIN entidade.funcaoentidade fe ON fe.fueid = f.fueid
                        LEFT JOIN entidade.entidade ORGAOFUNCAO ON ORGAOFUNCAO.entid = fe.entid
                        LEFT JOIN entidade.funentassoc f2 ON ent.entid = f2.entid
                        LEFT JOIN entidade.funcaoentidade fe2 ON fe2.fueid = f2.fueid
                        LEFT JOIN entidade.entidade COORD ON COORD.entid = fe2.entid
                        LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
                        LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod
                        LEFT JOIN pdeescola.meatividade mea ON mea.memid = mem.memid AND mea.meaano = mem.memanoreferencia
                        LEFT JOIN pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid AND mta.mtaanoreferencia = mem.memanoreferencia
                        LEFT JOIN pdeescola.metipomacrocampo mtm ON mtm.mtmid = mta.mtmid  
                        LEFT JOIN workflow.documento doc ON doc.docid = mem.docid
                        LEFT JOIN workflow.estadodocumento est ON est.esdid = doc.esdid
                        WHERE mem.memstatus = 'A' AND ende.muncod IS NOT NULL AND fe.funid = 19 AND fe2.funid = 41
                        --Estado
						".$where[0]."
						--Município.
						".$where[1]."
						--Ano
						".$where[2]."
    					--Esfera.
						".$where[3]."  
						--Macrocampo.
						".$where[4]."		
						--Pago FNDE.
						".$where[5]."
						--Escola Aberta
						".$where[6]."
						--Brasil sem miseria.
						".$where[7]."
						--Localização da Escola Rural ou Urbana.
						".$where[8]."
          				GROUP BY 
          					  mem.memid,ent.tpcid,ende.estuf,mun.mundescricao,ent.entcodent,ent.entnome,ende.endlog,ende.endcom,
               				  ende.endbai,ende.endnum,ent.entemail,ent.entnumdddcomercial,ent.entnumcomercial,ORGAOFUNCAO.entnome,
               				  ORGAOFUNCAO.entemail,ORGAOFUNCAO.entemail,ORGAOFUNCAO.entnumdddcomercial,ORGAOFUNCAO.entnumcomercial,
               				  ORGAOFUNCAO.entnumdddcelular,ORGAOFUNCAO.entnumcelular,ORGAOFUNCAO.entnumcpfcnpj,COORD.entnome,
               				  COORD.entemail, COORD.entnumdddcomercial,COORD.entnumcomercial,COORD.entnumdddcelular,
               				  COORD.entnumcelular,COORD.entnumcpfcnpj,mtm.mtmdescricao,mea.meaid,
               				  mtadescricao,mem.mempagofnde,mem.memvlrpago,mem.mamescolaaberta, mem.membrasilsemmiseria) AS ME
             LEFT JOIN (SELECT SUM(mp.mapquantidade) AS AlunosParticipantes, mp.memid 
                        FROM pdeescola.mealunoparticipante mp 
                        GROUP BY memid) AS AL ON AL.memid = ME.memid
    	     LEFT JOIN (SELECT sum(pa.mpaquantidade) AS AlunoAtividade, a.meaid AS meaid, me.memid AS memid
                        FROM  pdeescola.memaiseducacao me 
                        INNER JOIN pdeescola.meatividade a ON me.memid = a.memid
                        INNER JOIN pdeescola.mealunoparticipanteatividade pa ON pa.meaid = a.meaid 
                        --Ano
						".$where[9]."
                        GROUP BY a.meaid, me.memid ) AS AP ON AP.memid = ME.memid AND AP.meaid = ME.meaid
	        ORDER BY ME.estuf, ME.entcodent, ME.mundescricao, ME.entnome";
	//ver($sql,d); 
	return $sql;
}

function monta_coluna(){
	
	$colunas = $_POST['colunas'];
	$colunas = $colunas ? $colunas : array();
	
	$coluna = array();
	
	foreach ($colunas as $val){
		switch ($val) {
			case 'estuf':
				array_push( $coluna,
								array(	"campo"		=> "estuf",
										"label" 	=> "Unidade Federativa",
									   	"type"	  	=> "string"
								)
				);
				continue;
			break;
			case 'mundescricao':
				array_push( $coluna, 
								array(	"campo" 	=> "mundescricao",
							   		   	"label"		=> "Município",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'tpcid':
				array_push( $coluna, 
								array(	"campo" 	=> "tpcid",
							   		   	"label"		=> "Esfera",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'entcodent':
				array_push( $coluna, 
								array(	"campo" 	=> "entcodent",
							   		   	"label"		=> "Código INEP",
							   		   	"type"	  	=> "string"
								)
				);
				continue;
			break;
			case 'entnome':
				array_push( $coluna, 
								array(	"campo" 	=> "entnome",
							   		   	"label"		=> "Escola",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'endereco':
				array_push( $coluna, 
								array(	"campo" 	=> "endereco",
							   		   	"label"		=> "Endereço",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'entemail':
				array_push( $coluna, 
								array(	"campo" 	=> "entemail",
							   		   	"label"		=> "E-mail",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'telefone':
				array_push( $coluna, 
								array(	"campo" 	=> "telefone",
							   		   	"label"		=> "Telefone",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'mtmdescricao':
				array_push( $coluna, 
								array(	"campo" 	=> "mtmdescricao",
							   		   	"label"		=> "MacroCampo",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'mtadescricao':
				array_push( $coluna, 
								array(	"campo" 	=> "mtadescricao",
							   		   	"label"		=> "Atividade",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'diretor':
				array_push( $coluna, 
								array(	"campo" 	=> "diretor",
							   		   	"label"		=> "Diretor",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;			
			case 'cpf_diretor':
				array_push( $coluna, 
								array(	"campo" 	=> "cpf_diretor",
							   		   	"label"		=> "CPF Diretor",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'email_diretor':
				array_push( $coluna, 
								array(	"campo" 	=> "email_diretor",
							   		   	"label"		=> "E-mail Diretor",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;			
			case 'telefone_diretor':
				array_push( $coluna, 
								array(	"campo" 	=> "telefone_diretor",
							   		   	"label"		=> "Telefone Diretor",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;		
			case 'celular_diretor':
				array_push( $coluna, 
								array(	"campo" 	=> "celular_diretor",
							   		   	"label"		=> "Celular Diretor",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;			
			case 'coordenador':
				array_push( $coluna, 
								array(	"campo" 	=> "coordenador",
							   		   	"label"		=> "Coordenador",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;					
			case 'cpf_coordenador':
				array_push( $coluna, 
								array(	"campo" 	=> "cpf_coordenador",
							   		   	"label"		=> "CPF Coordenador",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;		
			case 'email_coordenador':
				array_push( $coluna, 
								array(	"campo" 	=> "email_coordenador",
							   		   	"label"		=> "E-mail Coordenador",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;					
			case 'telefone_coordenador':
				array_push( $coluna, 
								array(	"campo" 	=> "telefone_coordenador",
							   		   	"label"		=> "Telefone Coordenador",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;	
			case 'celular_coordenador':
				array_push( $coluna, 
								array(	"campo" 	=> "celular_coordenador",
							   		   	"label"		=> "Celular Coordenador",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;				
			case 'mempagofnde':
				array_push( $coluna, 
								array(	"campo" 	=> "mempagofnde",
							   		   	"label"		=> "Recursos FNDE",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'memvlrpago':
				array_push( $coluna, 
								array(	"campo" 	=> "memvlrpago",
							   		   	"label"		=> "Recurso pago",
							   		   	"type"	  	=> "numeric"
								) 
				);
				continue;
			break;			
			case 'mamescolaaberta':
				array_push( $coluna, 
								array(	"campo" 	=> "mamescolaaberta",
							   		   	"label"		=> "Escola Aberta",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'membrasilsemmiseria':
				array_push( $coluna, 
								array(	"campo" 	=> "membrasilsemmiseria",
							   		   	"label"		=> "Brasil sem miséria",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'alunos_participantes_total':
				array_push( $coluna, 
								array(	"campo" 	=> "alunos_participantes_total",
							   		   	"label"		=> "Total de Alunos",
							   		   	"type"	  	=> "numeric"
								) 
				);
				continue;
			break;	
			case 'alunos_participantes_atividade':
				array_push( $coluna, 
								array(	"campo" 	=> "alunos_participantes_atividade",
							   		   	"label"		=> "Total de Alunos Atividade",
							   		   	"type"	  	=> "numeric"
								) 
				);
				continue;
			break;						
		}
	}	
	return $coluna;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = 	array(	"agrupador" => array(), 
					"agrupadoColuna" => array("estuf","mundescricao","tpcid","entcodent","entnome","endereco","entemail","telefone","diretor","cpf_diretor","email_diretor","telefone_diretor","celular_diretor","coordenador","cpf_coordenador","email_coordenador","telefone_coordenador","celular_coordenador","mempagofnde","memvlrpago","mamescolaaberta","membrasilsemmiseria","alunos_participantes_total","alunos_participantes_atividade","mtmdescricao","mtadescricao")	  
			);
			
	$count = 1;
	$i = 0;

	foreach ($agrupador as $val){
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		} 
		switch ($val) {
			case 'estuf':
				array_push($agp['agrupador'], array("campo" => "estuf", "label" => "$var Estado") );				
		   		continue;
		    break;
		    case 'mundescricao':
				array_push($agp['agrupador'], array("campo" => "mundescricao", "label" => "$var Município") );					
		    	continue;
		    break;		    	
		    case 'entcodent':
				array_push($agp['agrupador'], array("campo" => "entcodent","label" => "$var INEP - Escola"));					
		   		continue;			
		    break;
		    case 'mtmdescricao':
				array_push($agp['agrupador'], array("campo" => "mtmdescricao","label" => "$var Macrocampo"));					
		   		continue;			
		    break;	
		    case 'mtadescricao':
				array_push($agp['agrupador'], array("campo" => "mtadescricao","label" => "$var Atividade"));					
		   		continue;			
		    break;			    
		}
		$count++;
	}
	return $agp;
}
?>
</body>
</html>