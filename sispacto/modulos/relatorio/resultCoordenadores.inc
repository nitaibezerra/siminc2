<?

include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();

//Gera arquivo xls
if( $_POST['tipo_relatorio'] == 'xls' ){
	
	$arCabecalho = array();
	$colXls = array();

	foreach($col as $cabecalho){
		array_push($arCabecalho, $cabecalho['label']);
		array_push($colXls, $cabecalho['campo']);
	}
	
	$arDados = Array();
		if(is_array($dados)){
	foreach( $dados as $k => $registro ){
		foreach( $colXls as $campo ){
			$arDados[$k][$campo] = $campo == 'slcnumsic' ? (string) '-'.trim($registro[$campo]).'-' : $registro[$campo];			
		}
	}
}
	
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=rel_coordenador_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=rel_coordenador_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%','');
	die;
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


$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao(true);
$r->setTotNivel(false);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(true);
$r->setTolizadorLinha(false);
$r->setBrasao(true);

if($_POST['tipo_relatorio'] == 'html'){
	echo $r->getRelatorio();
}

?>
</body>
</html>
<?php 
function monta_sql(){
	global $filtroSession;
	//ver($_POST);
	extract($_POST);
	
	$select = array();
	$from	= array();
	/*
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[1] = " AND foo.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}*/
	if( $municipio[0] && ( $municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[2] = " AND foo.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	//ver($where,d);
	if ($_POST['esfera']=='M'){
	   $sql = "SELECT 
       foo.rede, foo.estado,foo.muncod,
       foo.descricao, 
       foo.situacao, foo.coordenadorlocal, foo.coordenadoremail, '('||foo.coordenadordddtelefone||') ' || foo.coordenadortelefone as telefone,
       foo.termocompromisso,foo.total_orientadores_a_serem_cadastrados, foo.cadastrados, foo.porcentagem , 
       case when cadastrados > 0 then (total_orientadores_a_serem_cadastrados /cadastrados)::integer 
                    else 0 end as definicao_orientadores, professores                  
       FROM (
                               SELECT
                               case when p.estuf is not null then 'Estadual' else 'Municipal' end as rede,                                  
                               p.picstatus as status,
                               m.estuf as estado, 
                               m.mundescricao as descricao,
                               COALESCE(e.esddsc,'Não iniciou Elaboração') as situacao,
                               array_to_string(array(SELECT i.iusnome FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', ') as coordenadorlocal, 
                               array_to_string(array(SELECT i.iusemailprincipal FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', ') as coordenadoremail,
                               array_to_string(array(SELECT te.itedddtel::varchar FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd INNER JOIN sispacto.identificacaotelefone te ON te.iusd = i.iusd AND te.itetipo='T' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', ') as coordenadordddtelefone,
                               array_to_string(array(SELECT te.itenumtel::varchar FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd INNER JOIN sispacto.identificacaotelefone te ON te.iusd = i.iusd AND te.itetipo='T' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', ') as coordenadortelefone,
                               array_to_string(array(SELECT CASE WHEN iustermocompromisso=true THEN 'Sim' ELSE 'Não' END FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', ') as termocompromisso,


                               COALESCE((SELECT total_orientadores_a_serem_cadastrados FROM sispacto.totalalfabetizadores ta 
                                                WHERE ta.cod_municipio = p.muncod AND ta.dependencia = 'MUNICIPAL')) as  total_orientadores_a_serem_cadastrados, 

                               COALESCE((SELECT total FROM sispacto.totalalfabetizadores ta 
                                                WHERE ta.cod_municipio = p.muncod AND ta.dependencia = 'MUNICIPAL')) as  professores, -- professores
                                                

                                               (SELECT count(*) FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                WHERE i.picid=p.picid AND t.pflcod=827 AND i.iusstatus = 'A') as cadastrados,
                
                                         
                               COALESCE((SELECT AVG(apassituacao) FROM sispacto.atividadepacto at WHERE at.picid=p.picid AND at.suaid=2),0) as porcentagem,
                                               p.picstatus,
                                               m.estuf,
                                               p.muncod,
                                               e.esdid
                                               
                                               
                               FROM sispacto.pactoidadecerta p 
                               INNER JOIN territorios.municipio m ON m.muncod = p.muncod -- Municipio
                             --  INNER JOIN territorios.estado m ON m.estuf = p.estuf -- Estadual                               
                               LEFT JOIN workflow.documento d ON d.docid = p.docid  
                               LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid) foo 
                               WHERE foo.picstatus='A'".$where[2];
	   
 	   	
	//}
	// else{
	// }
	}
	else {
  	   $sql = "SELECT 
       foo.rede, foo.estado,foo.muncod,
     -- foo.descricao, 
       foo.situacao, foo.coordenadorlocal, foo.coordenadoremail, '('||foo.coordenadordddtelefone||') ' || foo.coordenadortelefone as telefone,
       foo.termocompromisso,foo.total_orientadores_a_serem_cadastrados, foo.cadastrados, foo.porcentagem , 
       case when cadastrados > 0 then (total_orientadores_a_serem_cadastrados /cadastrados)::integer 
                    else 0 end as definicao_orientadores, professores                  
       FROM (
                               SELECT
                               case when p.estuf is not null then 'Estadual' else 'Municipal' end as rede,                                  
                               p.picstatus as status,
                               m.estuf as estado, 
                           --    m.mundescricao as descricao,
                               COALESCE(e.esddsc,'Não iniciou Elaboração') as situacao,
                               
                               array_to_string(array(SELECT i.iusnome FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', ') as coordenadorlocal, 
                               array_to_string(array(SELECT i.iusemailprincipal FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', ') as coordenadoremail,
                               array_to_string(array(SELECT te.itedddtel::varchar FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd INNER JOIN sispacto.identificacaotelefone te ON te.iusd = i.iusd AND te.itetipo='T' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', ') as coordenadordddtelefone,
                               array_to_string(array(SELECT te.itenumtel::varchar FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd INNER JOIN sispacto.identificacaotelefone te ON te.iusd = i.iusd AND te.itetipo='T' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', ') as coordenadortelefone,
                               array_to_string(array(SELECT CASE WHEN iustermocompromisso=true THEN 'Sim' ELSE 'Não' END FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', ') as termocompromisso,

                                               
                               COALESCE((SELECT total_orientadores_a_serem_cadastrados FROM sispacto.totalalfabetizadores ta 
                                                WHERE ta.sigla = p.estuf AND ta.dependencia = 'ESTADUAL')) as  total_orientadores_a_serem_cadastrados, 

                               COALESCE((SELECT total FROM sispacto.totalalfabetizadores ta 
                                                WHERE ta.sigla = p.estuf AND ta.dependencia = 'ESTADUAL')) as  professores, -- professores
                                                

                                               (SELECT count(*) FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                WHERE i.picid=p.picid AND t.pflcod=827 AND i.iusstatus = 'A') as cadastrados,
                
                                         
                               COALESCE((SELECT apassituacao FROM sispacto.atividadepacto at WHERE at.picid=p.picid AND at.suaid=2),0) as porcentagem,
                                               p.picstatus,
                                               m.estuf,
                                               p.muncod,
                                               e.esdid
                                               
                                               
                               FROM sispacto.pactoidadecerta p 
                            --   INNER JOIN territorios.municipio m ON m.muncod = p.muncod -- Municipio
                               INNER JOIN territorios.estado m ON m.estuf = p.estuf -- Estadual                               
                               LEFT JOIN workflow.documento d ON d.docid = p.docid  
                               LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid) foo 
                               WHERE foo.picstatus='A' ";
 	   }
 
/*  else{
  	   $sql = "SELECT 
       foo.rede, foo.estado,foo.muncod,
      -- foo.descricao, 
       foo.situacao, foo.coordenadorlocal, foo.coordenadoremail, '('||foo.coordenadordddtelefone||') ' || foo.coordenadortelefone as telefone,
       foo.termocompromisso,foo.total_orientadores_a_serem_cadastrados, foo.cadastrados, foo.porcentagem , 
       case when cadastrados > 0 then (total_orientadores_a_serem_cadastrados /cadastrados)::integer 
                    else 0 end as definicao_orientadores, professores                  
       FROM (
                               SELECT
                               case when p.estuf is not null then 'Estadual' else 'Municipal' end as rede,                                  
                               p.picstatus as status,
                               m.estuf as estado, 
                           --    m.mundescricao as descricao,
                               COALESCE(e.esddsc,'Não iniciou Elaboração') as situacao,
                               
                               COALESCE((SELECT iusnome FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                 WHERE i.picid=p.picid AND t.pflcod=826),'Coordenador Local não cadastrado') as coordenadorlocal,
                                                 
                               COALESCE((SELECT iusemailprincipal FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                 WHERE i.picid=p.picid AND t.pflcod=826),'Email não cadastrado') as coordenadoremail,            

                               COALESCE((SELECT te.itedddtel::varchar FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                                                                                                                                             INNER JOIN sispacto.identificacaotelefone te ON te.iusd = i.iusd
                                                 WHERE i.picid=p.picid AND t.pflcod=826 AND te.itetipo = 'T'),'-') as coordenadordddtelefone,         

                               COALESCE((SELECT te.itenumtel::varchar FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                                                                                                                                                INNER JOIN sispacto.identificacaotelefone te ON te.iusd = i.iusd
                                                 WHERE i.picid=p.picid AND t.pflcod=826 AND te.itetipo = 'T'),'-') as coordenadortelefone,
                                                                                              
                               COALESCE((SELECT CASE WHEN iustermocompromisso=true THEN 'Sim' ELSE 'Não' END FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                         WHERE i.picid=p.picid AND t.pflcod=826),'Não') as termocompromisso,


                               COALESCE((SELECT total_orientadores_a_serem_cadastrados FROM sispacto.totalalfabetizadores ta 
                                                WHERE ta.cod_municipio = p.muncod AND ta.dependencia = 'MUNICIPAL')) as  total_orientadores_a_serem_cadastrados, 

                               COALESCE((SELECT total FROM sispacto.totalalfabetizadores ta 
                                                WHERE ta.cod_municipio = p.muncod AND ta.dependencia = 'MUNICIPAL')) as  professores, -- professores
                                                

                                               (SELECT count(*) FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                WHERE i.picid=p.picid AND t.pflcod=827 AND i.iusstatus = 'A') as cadastrados,
                
                                         
                               COALESCE((SELECT apassituacao FROM sispacto.atividadepacto at WHERE at.picid=p.picid AND at.suaid=2),0) as porcentagem,
                                               p.picstatus,
                                               m.estuf,
                                               p.muncod,
                                               e.esdid
                                               
                                               
                               FROM sispacto.pactoidadecerta p 
                            --   INNER JOIN territorios.municipio m ON m.muncod = p.muncod -- Municipio
                               INNER JOIN territorios.estado m ON m.estuf = p.estuf -- Estadual                               
                               LEFT JOIN workflow.documento d ON d.docid = p.docid  
                               LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid) foo 
                               WHERE foo.picstatus='A'";
  } */			
	/*else{
	
	$sql = "SELECT 
        foo.rede, foo.estado,foo.muncod, foo.descricao, 
       foo.situacao, foo.coordenadorlocal, foo.coordenadoremail, '('||foo.coordenadordddtelefone||') ' || foo.coordenadortelefone as telefone,
       foo.termocompromisso,foo.total_orientadores_a_serem_cadastrados, foo.cadastrados, foo.porcentagem , 
       case when cadastrados > 0 then (total_orientadores_a_serem_cadastrados /cadastrados)::integer 
                    else 0 end as definicao_orientadores, professores    
                    
                    
	--  foo.rede, foo.estado,foo.muncod, foo.descricao, foo.situacao, foo.coordenadorlocal, foo.coordenadoremail, 
      -- '('||foo.coordenadordddtelefone||') ' || foo.coordenadortelefone as telefone,
      -- foo.termocompromisso,foo.total_orientadores_a_serem_cadastrados, foo.cadastrados, foo.porcentagem , 
      -- case when cadastrados > 0 then (total_orientadores_a_serem_cadastrados /cadastrados)::integer 
      --              else 0 end as definicao_orientadores, professores
                  
       FROM (
                               SELECT 
                               case when p.estuf is not null then 'Estadual' else 'Municipal' end as rede,                                  
                               p.picstatus as status,
                               m.estuf as estado, 
                               m.mundescricao as descricao,
                               COALESCE(e.esddsc,'Não iniciou Elaboração') as situacao,
                               
                               COALESCE((SELECT iusnome FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                 WHERE i.picid=p.picid AND t.pflcod=826),'Coordenador Local não cadastrado') as coordenadorlocal,
                                                 
                               COALESCE((SELECT iusemailprincipal FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                 WHERE i.picid=p.picid AND t.pflcod=826),'Email não cadastrado') as coordenadoremail,            

                               COALESCE((SELECT te.itedddtel::varchar FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                                                                                                                                             INNER JOIN sispacto.identificacaotelefone te ON te.iusd = i.iusd
                                                 WHERE i.picid=p.picid AND t.pflcod=826 AND te.itetipo = 'T'),'-') as coordenadordddtelefone,         

                               COALESCE((SELECT te.itenumtel::varchar FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                                                                                                                                                INNER JOIN sispacto.identificacaotelefone te ON te.iusd = i.iusd
                                                 WHERE i.picid=p.picid AND t.pflcod=826 AND te.itetipo = 'T'),'-') as coordenadortelefone,
                                                                                              
                               COALESCE((SELECT CASE WHEN iustermocompromisso=true THEN 'Sim' ELSE 'Não' END FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                         WHERE i.picid=p.picid AND t.pflcod=826),'Não') as termocompromisso,


                               COALESCE((SELECT total_orientadores_a_serem_cadastrados FROM sispacto.totalalfabetizadores ta 
                                                WHERE ta.cod_municipio = p.muncod AND ta.dependencia = 'MUNICIPAL')) as  total_orientadores_a_serem_cadastrados, 

                               COALESCE((SELECT total FROM sispacto.totalalfabetizadores ta 
                                                WHERE ta.cod_municipio = p.muncod AND ta.dependencia = 'MUNICIPAL')) as  professores, -- professores
                                                

                                               (SELECT count(*) FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd 
                                                WHERE i.picid=p.picid AND t.pflcod=827 AND i.iusstatus = 'A') as cadastrados,
                
                                                         
                                         
                               COALESCE((SELECT apassituacao FROM sispacto.atividadepacto at WHERE at.picid=p.picid AND at.suaid=2),0) as porcentagem,
                                               p.picstatus,
                                               m.estuf,
                                               p.muncod,
                                               e.esdid
                                               
                                               
                               FROM sispacto.pactoidadecerta p 
                               INNER JOIN territorios.municipio m ON m.muncod = p.muncod -- Municipio
                               LEFT JOIN workflow.documento d ON d.docid = p.docid  
                               LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid) foo 
                               WHERE foo.picstatus='A'".$where[2]."".$where[1]."
                               ORDER BY foo.estuf, foo.descricao";

	}}*/

  return $sql;
}


function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"estado",
											"rede",
											"muncod",
											"coordenadorlocal",
											"coordenadoremail",
											"telefone",	
											"situacao",
											"termocompromisso",
											"total_orientadores_a_serem_cadastrados",
											"cadastrados",
											"porcentagem",
											"definicao_orientadores",
											"professores"
										  )	  
				);
				
				if ($_POST['esfera']=='E'){
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "Estado")										
									   				);
				}
				else{
					if($_POST['tipo_relatorio'] == 'xls'){
							array_push($agp['agrupador'], array(
									"campo" => "descricao",
							  		"label" => "Município")										
					   				);
					}
					else{	
						if ($_POST['esfera']=='M'){
							array_push($agp['agrupador'], array(
									"campo" => "estado",
									"label" => "Estado")										
									   				);
							array_push($agp['agrupador'], array(
									"campo" => "descricao",
							  		"label" => "Município")										
					   				);}
					   	else{ 
					   		array_push($agp['agrupador'], array(
								"campo" => "estado",
						  		"label" => "Estado")										
								);			
					   	}
					}
				}
	return $agp;
}

function monta_coluna(){

if ($_POST['esfera']=='E'){		
	$coluna    = array(
						
						array(
							  "campo" => "rede",
					   		  "label" => "Rede",
							  "type"  => "string"
						),
						array(
							  "campo" => "coordenadorlocal",
					   		  "label" => "Coordenador",
							  "type"  => "string"
						),
						array(
							  "campo" => "coordenadoremail",
					   		  "label" => "E-mail",
							  "type"  => "string"
						),
						array(
							  "campo" => "telefone",
					   		  "label" => "Telefone",
							  "type"  => "string"
						),
						array(
							  "campo" => "situacao",
					   		  "label" => "Situação",
							  "type"  => "string"
						),
						array(
							  "campo" => "termocompromisso",
					   		  "label" => "Termo de <br> Compromisso",
							  "type"  => "string"
						),
						array(
							  "campo" => "total_orientadores_a_serem_cadastrados",
					   		  "label" => "Total orientadores <br>a serem cadastrados",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "cadastrados",
					   		  "label" => "Cadastrados",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "porcentagem",
					   		  "label" => "Porcentagem",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "definicao_orientadores",
					   		  "label" => "Definição de <br> Orientadores",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "professores",
					   		  "label" => "Professores",
							  "type"  => "numeric"
						)
					);
}
else{
		if($_POST['tipo_relatorio'] == 'xls'){
		$coluna    = array(
								
								array(
									  "campo" => "muncod",
							   		  "label" => "Codigo do Município",
									  "type"  => "string"
								),
								array(
									  "campo" => "descricao",
							   		  "label" => "Município",
									  "type"  => "string"
								),
								array(
									  "campo" => "estado",
							   		  "label" => "Estado",
									  "type"  => "string"
								),
								array(
									  "campo" => "rede",
							   		  "label" => "Rede",
									  "type"  => "string"
								),
								array(
									  "campo" => "coordenadorlocal",
							   		  "label" => "Coordenador",
									  "type"  => "string"
								), 
								array(
									  "campo" => "coordenadoremail",
							   		  "label" => "E-mail",
									  "type"  => "string"
								),
								array(
									  "campo" => "telefone",
							   		  "label" => "Telefone",
									  "type"  => "string"
								),
								array(
									  "campo" => "situacao",
							   		  "label" => "Situação",
									  "type"  => "string"
								),
								array(
									  "campo" => "termocompromisso",
							   		  "label" => "Termo de <br> Compromisso",
									  "type"  => "string"
								),
								array(
									  "campo" => "total_orientadores_a_serem_cadastrados",
							   		  "label" => "Total orientadores <br>a serem cadastrados",
									  "type"  => "numeric"
								),
								array(
									  "campo" => "cadastrados",
							   		  "label" => "Cadastrados",
									  "type"  => "numeric"
								),
								array(
									  "campo" => "porcentagem",
							   		  "label" => "Porcentagem",
									  "type"  => "numeric"
								),
								array(
									  "campo" => "definicao_orientadores",
							   		  "label" => "Definição de <br> Orientadores",
									  "type"  => "numeric"
								),
								array(
									  "campo" => "professores",
							   		  "label" => "Professores",
									  "type"  => "numeric"
								)
							);
		}
		else{
		$coluna    = array(
								array(
									  "campo" => "rede",
							   		  "label" => "Rede",
									  "type"  => "string"
								),
								array(
									  "campo" => "coordenadorlocal",
							   		  "label" => "Coordenador",
									  "type"  => "string"
								),
								array(
									  "campo" => "coordenadoremail",
							   		  "label" => "E-mail",
									  "type"  => "string"
								),
								array(
									  "campo" => "telefone",
							   		  "label" => "Telefone",
									  "type"  => "string"
								),
								array(
									  "campo" => "situacao",
							   		  "label" => "Situação",
									  "type"  => "string"
								),
								array(
									  "campo" => "termocompromisso",
							   		  "label" => "Termo de <br> Compromisso",
									  "type"  => "string"
								),
								array(
									  "campo" => "total_orientadores_a_serem_cadastrados",
							   		  "label" => "Total orientadores <br>a serem cadastrados",
									  "type"  => "numeric"
								),
								array(
									  "campo" => "cadastrados",
							   		  "label" => "Cadastrados",
									  "type"  => "numeric"
								),
								array(
									  "campo" => "porcentagem",
							   		  "label" => "Porcentagem",
									  "type"  => "numeric"
								),
								array(
									  "campo" => "definicao_orientadores",
							   		  "label" => "Definição de <br> Orientadores",
									  "type"  => "numeric"
								),
								array(
									  "campo" => "professores",
							   		  "label" => "Professores",
									  "type"  => "numeric"
								)
							);	
		}
}
	return $coluna;			  	

}
?>
