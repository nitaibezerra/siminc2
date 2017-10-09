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
	header ( "Content-type: application/xls; name=rel_orientadores_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=rel_orientadores_".date("Ymdhis").".xls");
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

/*
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
*/
if($_POST['tipo_relatorio'] == 'html'){
$arCabecalho = array();
	$colXls = array();

	foreach($col as $cabecalho){
		array_push($arCabecalho, $cabecalho['label']);
		array_push($colXls, $cabecalho['campo']);
	}
	echo '<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">'
				.'	<tr bgcolor="#ffffff">' 	
				.'		<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>'			
				.'		<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">'				
				.'			'. NOME_SISTEMA. '<br/>'				
				.'			MEC / SE - Secretaria Executiva <br />'
				.'		</td>'
				.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'					
				.'			Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>'					
				.'			Hora da Impressão:' . date( 'd/m/Y - H:i:s' ) . '<br />'					
				.'		</td>'					
				.'	</tr>'					
				.'</table>';
	$db->monta_lista($sql,$arCabecalho,501,5,$soma,"100%","S");
	
}

?>
</body>
</html>
<?php 
function monta_sql(){
	global $filtroSession;
	
	extract($_POST);
	
	$select = array();
	$from	= array();
	
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[1] = " AND tm.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ( $municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[2] = " AND tm.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	
	if ($_POST['esfera']=='M'){
		$sql ="select

                'Municipal' as rede,
                tm.estuf as uf_atuacao,
				tm.muncod,
                tm.mundescricao as municipio_atuacao,
                m.estuf as uf_pacto,
                m.muncod as cod_mpacto,
                m.mundescricao as municipio_pacto,                
                i.iuscpf as cpf,
                i.iusnome as nome,
                i.iusemailprincipal as email_principal,
                case when t.pflcod=827 then
							                case when i.iustipoorientador = 'tutoresproletramento' then 'Tutores do Pró-Letramento'
							                     when i.iustipoorientador = 'tutoresredesemproletramento' then 'Tutores Sem Pró-Letramento'
							                     when i.iustipoorientador = 'profissionaismagisterio' then 'Profissionais do Magistério'
							                     else 'Não casdastrado' end 
                     when t.pflcod=849 then
							                case when i.iustipoprofessor = 'cpflivre' then 'Não bolsista'
							                     when i.iustipoprofessor = 'censo' then 'Bolsista'
							                     else 'Não casdastrado' end 
                 end as tipo,
                 e.esddsc as situacao
                
			from
					sispacto.identificacaousuario i
			left join
					sispacto.pactoidadecerta p on p.picid = i.picid
			left join
					territorios.municipio m on m.muncod = p.muncod
			left join
					territorios.municipio tm on tm.muncod = i.muncodatuacao
			left join
					sispacto.tipoperfil t on i.iusd=t.iusd 
			inner join
					workflow.documento d on d.docid = p.docid 
			inner join
					workflow.estadodocumento e on e.esdid = d.esdid
			where 
					t.pflcod = ".$pflcod." 
			and 
					i.iusstatus = 'A'
			and 
					p.estuf is  null".$where[2];
			
			
	}
	else{
		$sql = "select
       'Estadual' as rede,
       tm.estuf as uf_atuacao,                
       tm.mundescricao as municipio_atuacao,
       m.estdescricao as uf_pacto,    
       --'' as municipio_pacto,                                           
       i.iuscpf as cpf,
       i.iusnome as nome,
       i.iusemailprincipal as email_principal,
                case when t.pflcod=827 then
							                case when i.iustipoorientador = 'tutoresproletramento' then 'Tutores do Pró-Letramento'
							                     when i.iustipoorientador = 'tutoresredesemproletramento' then 'Tutores Sem Pró-Letramento'
							                     when i.iustipoorientador = 'profissionaismagisterio' then 'Profissionais do Magistério'
							                     else 'Não casdastrado' end 
                     when t.pflcod=849 then
							                case when i.iustipoprofessor = 'cpflivre' then 'Não bolsista'
							                     when i.iustipoprofessor = 'censo' then 'Bolsista'
							                     else 'Não casdastrado' end 
                 end as tipo,
       
       e.esddsc as situacao,
		tm.muncod
                                                      
		from
				sispacto.identificacaousuario i
		left join
				sispacto.pactoidadecerta p on p.picid = i.picid
		left join
		   	        territorios.estado m on m.estuf = p.estuf
		left join
				territorios.municipio tm on tm.muncod = i.muncodatuacao
		left join
				sispacto.tipoperfil t on i.iusd=t.iusd 
		inner join
				workflow.documento d on d.docid = p.docid 
		inner join
				workflow.estadodocumento e on e.esdid = d.esdid                          
		where 
				t.pflcod = ".$pflcod." 
		and 
				i.iusstatus = 'A'
		and 
				p.estuf is not null".$where[1];
	}
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"rede",
											"estado",
											"descricao",
											"uf_atuacao",
											"municipio_atuacao",
											"uf_pacto",
											"municipio_pacto",
											"cpf",
											"nome",
											"email_principal",
											"tipo",
											"estado",
											"muncod",
												
										  )	  
				);
				array_push($agp['agrupador'], array(
													"campo" => "rede",
											  		"label" => "Rede")										
									   				);			
				
				//array_push($agp['agrupador'], array(
				//									"campo" => "estado",
				//							  		"label" => "Estado")										
				//					   				);				
	return $agp;
}

function monta_coluna(){
	if ($_POST['esfera']=='M'){
	$coluna    = array(
						
						array(
							  "campo" => "rede",
					   		  "label" => "Rede",
							  "type"  => "string"
						),
						array(
							  "campo" => "uf_atuacao",
					   		  "label" => "UF Atuação",
							  "type"  => "string"
						),
						array(
							  "campo" => "muncod",
					   		  "label" => "Cod. Município Atuação",
							  "type"  => "string"
						
						),
						array(
							  "campo" => "municipio_atuacao",
					   		  "label" => "Município de Atuação",
							  "type"  => "string"
						),
						array(
							  "campo" => "uf_pacto",
					   		  "label" => "UF Pacto",
							  "type"  => "string"
						),
						array(
							  "campo" => "cod_mpacto",
					   		  "label" => "Cod. Município Pacto",
							  "type"  => "string"
						),
						array(
							  "campo" => "municipio_pacto",
					   		  "label" => "Município Pacto",
							  "type"  => "string"
						),
						array(
							  "campo" => "cpf",
					   		  "label" => "CPF",
							  "type"  => "string"
						),
						array(
							  "campo" => "nome",
					   		  "label" => "Nome",
							  "type"  => "string"
						
						),
						array(
							  "campo" => "email_principal",
					   		  "label" => "E-mail Principal",
							  "type"  => "string"
						),
						array(
							  "campo" => "tipo",
					   		  "label" => "Tipo",
							  "type"  => "string"
						),
						array(
							  "campo" => "estado",
					   		  "label" => "Estado",
							  "type"  => "string"
						
						),

					);
	}
	else {
		$coluna    = array(
						
						array(
							  "campo" => "rede",
					   		  "label" => "Rede",
							  "type"  => "string"
						),
						array(
							  "campo" => "uf_atuacao",
					   		  "label" => "UF Atuação",
							  "type"  => "string"
						),
						array(
							  "campo" => "municipio_atuacao",
					   		  "label" => "Município de Atuação",
							  "type"  => "string"
						),
						array(
							  "campo" => "uf_pacto",
					   		  "label" => "UF Pacto",
							  "type"  => "string"
						),
						array(
							  "campo" => "cpf",
					   		  "label" => "CPF",
							  "type"  => "string"
						),
						array(
							  "campo" => "nome",
					   		  "label" => "Nome",
							  "type"  => "string"
						
						),
						array(
							  "campo" => "email_principal",
					   		  "label" => "E-mail Principal",
							  "type"  => "string"
						),
						array(
							  "campo" => "tipo",
					   		  "label" => "Tipo",
							  "type"  => "string"
						),
						array(
							  "campo" => "estado",
					   		  "label" => "Estado",
							  "type"  => "string"
						
						),
						array(
							  "campo" => "muncod",
					   		  "label" => "Código do Município",
							  "type"  => "string"
						
						)
					);
	}
	return $coluna;			  	

}
?>
