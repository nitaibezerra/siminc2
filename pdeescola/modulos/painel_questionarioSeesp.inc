<?
ini_set('memory_limit','1024M');


//Monta o cabeçalho e título da tela
include  APPRAIZ."includes/cabecalho.inc";
echo"<br>";
monta_titulo("Painel", "Questionário SEESP");

?>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th>
			<a style="cursor:pointer;" onclick="window.location.href='/pdeescola/pdeescola.php?modulo=questionario&acao=D'">Acessar questionários SEESP</a>
		</th>
	</tr>
	<tr>
		<td valign="top">
				<!-- Questionário SEESP -->
				<?php
				include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao(false);
$r->setTotNivel(true);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(true);
$r->setTotalizador(true);
$r->setTolizadorLinha(true);

//$r->setBrasao(true);
echo $r->getRelatorio();
?>
</body>
</html>
<?php 
function monta_sql(){
	global $filtroSession;
	
	extract($_POST);
	
	$select = array();
	$from	= array();

	
	$sql = "SELECT 
								f.questionario, 
								f.descricao, 
								f.municipio, 
								f.escolas, 
								SUM(f.total) as total
							FROM (
							SELECT
								qq.quetitulo as questionario,
								ee.estuf as estado, 
								est.estdescricao as descricao,
								m.mundescricao as municipio,
								e.entnome as escolas,
								case when qq.queid = ".QUESTIONARIO_VI." then
									case when ( count(qr.resid) = 14 ) then 1 else 0 end 
								else
									case when ( count(qr.resid) > 12 ) then 1 else 0  end 
								end as total
							FROM
								pdeescola.pdequestionario pq
							INNER JOIN questionario.questionarioresposta qqr ON qqr.qrpid = pq.qrpid
							INNER JOIN questionario.questionario qq ON qq.queid = qqr.queid
							INNER JOIN questionario.resposta qr ON qr.qrpid = pq.qrpid
							INNER JOIN entidade.endereco ee ON ee.entid = pq.entid 
							INNER JOIN entidade.entidade e ON e.entid = ee.entid
							INNER JOIN territorios.municipio m ON m.muncod = ee.muncod
							INNER JOIN territorios.estado est ON est.estuf = m.estuf
							WHERE
								perid in ( select perid from questionario.pergunta where grpid in ( select grpid from questionario.grupopergunta where queid IN ( ".QUESTIONARIO_VI.", ".QUESTIONARIO_VII." )  ) )
							--	AND SUBSTRING(qqr.qrptitulo,length(qqr.qrptitulo) - 4,4) = '".$_SESSION['exercicio']."'
							GROUP BY
								qq.queid, qq.quetitulo, ee.estuf, est.estdescricao, m.mundescricao, e.entnome
							ORDER BY
								qq.quetitulo, ee.estuf, est.estdescricao, m.mundescricao, e.entnome
								
								) as f
							
							WHERE
								total = 1
							GROUP BY
								f.questionario,
								f.descricao, 
								f.municipio,
								f.escolas
							ORDER BY
								f.questionario,
								f.descricao, 
								f.municipio,
								f.escolas";
	
//ver($sql);
	return $sql;
}

function monta_agp(){
	$agrupador = Array( 0 => 'questionario', 1 => 'estado', 2 => 'municipio', 3 => 'escolas' );
	
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"total"
									//		"qnt2",
										//	"percent"
											/*"projetado",	
									   		"autorizado", 
 									   		"provimento",
 									   		"publicado",
 									   		"homologado",
											"lepvlrprovefetivados",
											"provimentosnaoefetivados",
											"provimentopendencia",
										    "homocolor"
										    */
										  )	  
				);
	
	$count = 1;
		$i = 0;
		
		if( $i > 1 || $i == 0 ){
			$vari = "ESCOLA - Questionário SEESP<br>";	
		}
	foreach ($agrupador as $val):
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "descricao",
											  		"label" => "$var Estado")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "$var Município")										
									   				);					
		    	continue;
		    break;		    	
		    case 'questionario':
				array_push($agp['agrupador'], array(
													"campo" => "questionario",
											 		"label" => "$var Questionário")										
									   				);					
		    	continue;			
		    break;
		    case 'escolas':
				array_push($agp['agrupador'], array(
													"campo" => "escolas",
											 		"label" => "$var Escola")										
									   				);					
		   		continue;			
		    break;
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna(){
	$coluna    = array(
						array(
							  "campo" => "total",
					   		  "label" => "Questionário Completo",
							  "type"  => "numeric"
							 // "html"  	 => "<span style='color:{color}'>{equipe}</span>",
						)
	//					array(
	//						  "campo" => "autorizado",
	//				   		  "label" => "Autorizado",
	//						  "type"  => "numeric"
	//					),
//						array(
//							  "campo" => "qnt2",
//					   		  "label" => "Quantidade Atendida",
//							  "type"  => "numeric"
//						),
//						array(
//							  "campo" => "percent",
//					   		  "label" => "Percentual <br>de Defasagem (%)",
//							  "type"  => "numeric"
//						)
					);
		/*			
	if ( $permissao ){				
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric",
								  "html"  	 => "<span style='color:{color}'>{homologado}</span>",
								  "php" 	 => array(
									  					"expressao" => "{homologado} > {autorizado}",
														"var" 		=> "color",
														"true"	    => "red",
														"false"     => "#0066CC",
									  				 )	
							)
				  );			
	}else{
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric"
								 )
				  );			

	}
	
	array_push($coluna,array(
							  "campo" => "provimento",
					   		  "label" => "Provimentos <br/>Autorizados <br/>(E)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "lepvlrprovefetivados",
					   		  "label" => "Provimentos <br/>Efetivados <br/>(F)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "provimentosnaoefetivados",
					   		  "label" => "Provimentos <br/>Não Efetivados <br/>(G)=(E)-(F)",
							  "type"  => "numeric"
						)
				);	

				
	if ( $permissao ){				
		array_push($coluna, array(
									  "campo" => "provimentopendencia",
							   		  "label" => "Aut. Provimento <br/>Pendente <br/>(H)=(D)-(E)",
									  "type"  => "numeric",
									  "html"  => "{valorcampo}",
									  "php"   => array(
									  					"expressao" => "({homologado} - {provimento} >= 0) || ({provimentopendencia} > 0)",
														"type"		=> "string",
									  					"var"       => "valorcampo",
														"true"		=> "{provimentopendencia}",
														"false"		=> "<span style='color:red;'>-</span>"					
									  				   )
								)
				  );			
	}else{
		array_push($coluna, array(
									  "campo" => "provimentopendencia",
							   		  "label" => "Aut. Provimento <br/>Pendente <br/>(H)=(D)-(E)",
									  "type"  => "numeric"		
								  )
				  );			

	}		*/	

	return $coluna;	
}
				?>
			</td>
	</tr>
</table>