<?

ini_set("memory_limit","-1");
set_time_limit(0);

// OBS: deletar parecerinstrumento

require_once APPRAIZ . 'cte/classes/ParecerPar.class.inc';
/************************* ENVIAR DADOS ******************************/
if( count( $_SESSION['validaErro'] )){
	include APPRAIZ ."includes/enviar.inc";
	echo "<script> 
			window.close();
		  </script>";
	exit;
}

if($_REQUEST['salvar']){
	if($_REQUEST['parecer1'] || $_REQUEST['parecer2']){
		include APPRAIZ ."includes/enviar.inc";
		die();
	}else{
		echo "<script> 
			alert('Selecione um Parecer.');
			window.close();
		  </script>";
		exit;
	}
}

/************************* CABEÇALHO E TÍTULO ******************************/
$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );
$erro = '';

$cabecalho = array("Ação","Localização", "Subação","Tipo","Plano Interno", "UF", "Valor","Ano");
$listaSubacoes = recuperaSubacoes();
if(!is_array($listaSubacoes)){
	$listaSubacoes[0][0] = '';
	$erro = "Não existem subações a serem enviadas.";
}

$cabecalhoParecer = array("Ação","Tipo", "Parecer","Data","Feito por");
$listaPareceres = recuperaParecer();

?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
</head>
	<body>
		<form name="formulario" id="formulario" action="" method="post">
			<input type=hidden name="visualiza" id="visualiza" value="false" />
			<input type=hidden name="salvar" value="false" />
			<table align="center" border="0" class="ta		spt.sptano AS bela" cellpadding="3" cellspacing="1" width="100%";>
				<tr>
					<th>OBS: As subações a serem enviadas devem conter: O parecer dado, ser aprovado pela comissão e conter o plano interno.</th>
				</tr>
				<tr>
					<td>Selecione as subações para enviar serem enviadas ao FNDE</td>
				</tr>
				<tr>
					<th>Lista de Subações aprovadas pela comissão e de assitencia Financeira</th>
				</tr>
				<tr>
					<?php 
					if($erro){
					?>
						<td>
						<div style="text-align: center; color: red;"><?php echo $erro; ?></div>
						<br>
						</td>
					<?php 
					}else{
					?>
						<td>
							<?php $db->monta_lista_simples($listaSubacoes,$cabecalho,50, 10, 'N', '100%', ''); ?>
						<br></td>
					<?php } ?>
				</tr>
				<tr>
					<th> 
						Lista de Pareceres
					</th>
				</tr>
				<tr>
					<td>
						<?php $db->monta_lista_simples($listaPareceres,$cabecalhoParecer,20, 10, 'N', '100%', ''); ?>
					</td>
				</tr>
				<tr>
					<th> 
						Justificativa do PTA
					</th>
				</tr>
				<tr>
					<td>
						<input type="radio" name="prsjustificativapta" id="justificativaPadrao" value="padrao" checked="checked" /><label for="justificativaPadrao">Justificativa Padrão</label><br />
						<input type="radio" name="prsjustificativapta" id="justificativaCaminhoEscola" value="caminhoEscola" /><label for="justificativaCaminhoEscola">Justificativa Caminho da Escola</label>
					</td>
				</tr>				
				<tr>
					<td>
						<input type="button" name="salvar" value="Enviar"  onclick="salvarDados(0);" />
						<input type="button" name="visualizar" value="Visualizar Projeto"  onclick="salvarDados(1);" />
						<input type="button" name="fechar" id="fechar" value="fechar"  onclick="fecharTela();" />
					</td>
				</tr>
			</table>
		</form>
		<script language="javascript">
			/**
			 * function salvarDados();
			 * descscricao 	: Valida dados e submete o formulario;
			 */
			function salvarDados(flag){
				var checado = false;
				var msn 	= '';
				var parecerFinanceiro = 0;
				var parecerEngenharia = 0;

				var justificativaPadrao = document.getElementById( 'justificativaPadrao' ).value;
				var justificativaCaminhoEscola = document.getElementById( 'justificativaCaminhoEscola' ).value;

				if( flag == '1' ){
					document.getElementById('visualiza').value = 'true';
				}
				
				for (i=0;i<document.formulario.elements.length;i++){ 
      				if(document.formulario.elements[i].type == "checkbox"){
      					if( (document.formulario.elements[i].id != "parecer1") && (document.formulario.elements[i].id != "parecer2") ){
	      					if ( document.formulario.elements[i].checked ) { 
	      						checado = true;
	      						msn 	+= document.formulario.elements[i].title+'\n ';
	      					}
      					}else{
      						if( document.formulario.elements[i].checked){
      							if(document.formulario.elements[i].id == "parecer1" ){
									parecerFinanceiro++;
      							}else if(document.formulario.elements[i].id == "parecer2"){
      								parecerEngenharia++;
      							}
      						}
      					}
					}
				}
				if( (parecerFinanceiro == '') && ( parecerEngenharia == '') ){
					alert("Não existe parecer cadastrado.");
					return false;
				}
				if( (parecerFinanceiro >= 1) && ( parecerEngenharia >= 1) ){
					alert("Só pode ser selecionado apenas um parecer.");
					return false;
				}
				if( !justificativaPadrao && !justificativaCaminhoEscola ){
					alert("Favor preencher a Justificativa do PTA.");
					return false;
				}
				
				if(checado){
					if(confirm("Deseja realmente enviar as seguintes subações: \n"+msn+" para o FNDE? ")){
						document.formulario.getElementsByTagName('salvar').value = true;
						document.formulario.submit();
					}
				}else {
					alert("Selecione pelo menos uma subação para ser enviada para o FNDE.");
				}
			}
			
			function fecharTela(){
				window.close();
			}
		</script>
	</body>
</html>
<?php
	/*******************************
	* 	FUNCTION: recuperaPlanointernoAcaoSubacao();
	* 	22/06/2009
	*   CRIADA POR: Thiago Tasca
	*   DESCRIÇÃO:
	*	Recupera os as subações que irão gerar convênio
	* 
	*******************************/
	function recuperaSubacoes(){
		global $db;
		
		$municipio = cte_pegarMuncod( $_SESSION["inuid"] );
		if($municipio != NULL){ 		//*** Municipio ***//
			$select  = " iu.mun_estuf AS uf,"; 
			$from    = "INNER JOIN territorios.municipio  m  ON m.muncod = iu.muncod";
			$where   = "m.muncod = '".$municipio."' AND"; 
			
		}else{ 							//*** Estado ***//
			$estado	= cte_pegarMuncodEstatual($_SESSION["inuid"]);
			$itrid 	= cte_pegarItrid($_SESSION["inuid"]);  
			
			$select = " iu.estuf AS uf,"; 
			$from   = "INNER JOIN territorios.estado m ON m.estuf = iu.estuf";
			$where  = "m.muncodcapital 	= '".$estado."' 
					   AND iu.inuid 	= '".$_SESSION["inuid"]."' 
					   AND iu.itrid 	= '".$itrid."' AND";
		}
		
			/*$sql= "
				SELECT distinct
						'<input name=\"checkbox[]\" id=\"checkbox[]\" type=\"checkbox\" title=\"'||s.sbadsc||'\" value=\"'||s.sbaid||'\"/>' AS acao,
						d.dimcod ||'.'|| ad.ardcod ||'.'|| i.indcod	AS localizacao, 
						s.sbadsc						AS descricaosubacao,
						CASE WHEN s.sbaidpai is not null THEN 'Aditivo' ELSE 'Original' END AS tipo,
						spt.sptplanointerno				AS pi,
						m.estuf 						AS uf,
						'' as valor,
						s.sbaporescola	AS porescola,
						s.sbaid
						
				FROM cte.dimensao d
					INNER JOIN cte.areadimensao 	  ad  ON ad.dimid = d.dimid
					INNER JOIN cte.indicador 	  	  i   ON i.ardid  = ad.ardid
					INNER JOIN cte.criterio		  	  c   ON c.indid  = i.indid
					INNER JOIN cte.pontuacao 	  	  p   ON p.crtid  = c.crtid and p.indid = i.indid and p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade iu  ON iu.inuid = p.inuid
					".$from."
					INNER JOIN cte.acaoindicador 	  		a   ON a.ptoid   = p.ptoid
					INNER JOIN cte.subacaoindicador   		s   ON s.aciid   = a.aciid
					INNER JOIN cte.subacaoparecertecnico 	spt ON spt.sbaid = s.sbaid and trim(spt.sptplanointerno) <> '' --and sptano = date_part('year', current_date) 
					INNER JOIN cte.unidademedida 	  		u   ON u.undid   = s.undid
					--INNER JOIN cte.programa 	  	  pr  ON pr.prgid = s.prgid   and trim(pr.prgplanointerno) <> ''
                    LEFT  JOIN cte.convenioretorno    cvr ON ( cvr.inuid = iu.inuid and spt.sptplanointerno = cvr.prgplanointerno )
					LEFT JOIN cte.parecerpar 	  pp  ON pp.inuid = iu.inuid
					--LEFT JOIN seguranca.usuario 	  su  ON su.usucpf = pp.usucpf
				WHERE
					 ".$where."
					 s.frmid in(11,3,2)  -- assistencia financeira.
					 AND spt.ssuid = 3   -- aprovada pela comissão.
					 AND pp.tppid = 1	 -- parecer financeiro dado
					-- AND parhistorico in(SELECT max(parhistorico) FROM cte.parecerpar p WHERE date_part('year',pardata)= date_part('year', current_date) AND p.inuid = iu.inuid ) -- ultimo parecer 
					 AND date_part('year', pp.pardata)= date_part('year', current_date) -- parecer do ano corrente	
				ORDER BY 
					s.sbadsc ";*/

		$sql = "SELECT 
					acao,
					localizacao,
					descricaosubacao,
					tipo,
					pi,
					uf,
					valor,
					porescola,
					sbaid,
					ano
				FROM
				(
				SELECT distinct
							'<input name=\"checkbox[]\" id=\"checkbox[]\" type=\"checkbox\" title=\"'||s.sbadsc||'\" value=\"'||s.sbaid||'\"/>' AS acao,
							d.dimcod ||'.'|| ad.ardcod ||'.'|| i.indcod	AS localizacao, 
							s.sbadsc						AS descricaosubacao,
							CASE WHEN s.sbaidpai is not null THEN 'Aditivo' ELSE 'Original' END AS tipo,
							spt.sptplanointerno				AS pi,
							-- m.estuf 						AS uf,
							".$select."
							'' as valor,
							s.sbaporescola	AS porescola,
							s.sbaid,
							spt.sptano AS ano
					FROM cte.dimensao d
						INNER JOIN cte.areadimensao 	  ad  ON ad.dimid = d.dimid
						INNER JOIN cte.indicador 	  	  i   ON i.ardid  = ad.ardid
						INNER JOIN cte.criterio		  	  c   ON c.indid  = i.indid
						INNER JOIN cte.pontuacao 	  	  p   ON p.crtid  = c.crtid and p.indid = i.indid and p.ptostatus = 'A'
						INNER JOIN cte.instrumentounidade iu  ON iu.inuid = p.inuid
						".$from."
						-- INNER JOIN territorios.municipio  m  ON m.muncod = iu.muncod
						INNER JOIN cte.acaoindicador 	  		a   ON a.ptoid   = p.ptoid
						INNER JOIN cte.subacaoindicador   		s   ON s.aciid   = a.aciid
						INNER JOIN cte.subacaoparecertecnico 	spt ON spt.sbaid = s.sbaid and trim(spt.sptplanointerno) <> '' and sptano = date_part('year', current_date) 
						INNER JOIN cte.unidademedida 	  		u   ON u.undid   = s.undid
				LEFT  JOIN cte.convenioretorno    cvr ON ( cvr.inuid = iu.inuid and spt.sptplanointerno = cvr.prgplanointerno )
						LEFT JOIN cte.parecerpar 	  pp  ON pp.inuid = iu.inuid
					WHERE
						  ".$where."
						 s.frmid in(11,3,2)  -- assistencia financeira.
						 AND spt.ssuid = 3   -- aprovada pela comissão.
						 AND pp.tppid = 1	 -- parecer financeiro dado
						 AND date_part('year', pp.pardata)= date_part('year', current_date) -- parecer do ano corrente	
					
					UNION ALL
				
						SELECT distinct
							'<input name=\"checkbox[]\" id=\"checkbox[]\" type=\"checkbox\" title=\"'||s.sbadsc||'\" value=\"'||s.sbaid||'\"/>' AS acao,
							d.dimcod ||'.'|| ad.ardcod ||'.'|| i.indcod	AS localizacao, 
							s.sbadsc						AS descricaosubacao,
							CASE WHEN s.sbaidpai is not null THEN 'Aditivo' ELSE 'Original' END AS tipo,
							spt.sptplanointerno				AS pi,
							-- m.estuf 						AS uf,
							".$select."
							'' as valor,
							s.sbaporescola	AS porescola,
							s.sbaid,
							spt.sptano AS ano
							
					FROM cte.dimensao d
						INNER JOIN cte.areadimensao 	  ad  ON ad.dimid = d.dimid
						INNER JOIN cte.indicador 	  	  i   ON i.ardid  = ad.ardid
						INNER JOIN cte.criterio		  	  c   ON c.indid  = i.indid
						INNER JOIN cte.pontuacao 	  	  p   ON p.crtid  = c.crtid and p.indid = i.indid and p.ptostatus = 'A'
						INNER JOIN cte.instrumentounidade iu  ON iu.inuid = p.inuid
						".$from."
						-- INNER JOIN territorios.municipio  m  ON m.muncod = iu.muncod
						INNER JOIN cte.acaoindicador 	  		a   ON a.ptoid   = p.ptoid
						INNER JOIN cte.subacaoindicador   		s   ON s.aciid   = a.aciid
						INNER JOIN cte.subacaoparecertecnico 	spt ON spt.sbaid = s.sbaid and trim(spt.sptplanointerno) <> '' --and sptano = date_part('year', current_date) 
						INNER JOIN cte.unidademedida 	  		u   ON u.undid   = s.undid
						LEFT  JOIN cte.convenioretorno    cvr ON ( cvr.inuid = iu.inuid and spt.sptplanointerno = cvr.prgplanointerno )
						LEFT JOIN cte.parecerpar 	  pp  ON pp.inuid = iu.inuid
					WHERE
						  ".$where."
						 s.frmid in(11,3,2)  -- assistencia financeira.
						 AND spt.ssuid = 14   -- aprovada pela comissão.
						 AND pp.tppid = 1	 -- parecer financeiro dado
						 AND date_part('year', pp.pardata)= date_part('year', current_date) -- parecer do ano corrente	
						 AND s.sbaidpai is not null
					
				) as foo
				
					ORDER BY 
						descricaosubacao";
		//dbg($sql,1);
		$arrayDados = $db->carregar($sql);
		
		if(is_array($arrayDados)){
			//foreach($arrayDados as $valores){
			for ($cont = 0; $cont < count($arrayDados); $cont++){
				$valor = recuperavaloresgeraislista($arrayDados[$cont]['sbaid'], $arrayDados[$cont]['porescola'], $arrayDados[$cont]['ano']);
				$valor = number_format($valor, 2, ',', '.');
				$arrayDados[$cont]['valor'] = $valor; 
				unset($arrayDados[$cont]['porescola']);
				unset($arrayDados[$cont]['sbaid']);
			}
			
		}
		return  $arrayDados;//$sql;
	}
	
	function recuperaParecer(){
		global $db;
		$inuid = $_SESSION['inuid'];
		$sqlListaFinanceiro = "	SELECT  
									'<input name=\"parecer'||tppid||'\" id=\"parecer'||tppid||'\" type=\"checkbox\" title=\"'||parid||'\" value=\"'||parid||'\"/>' AS acao,
									CASE WHEN tppid = 1 THEN 'Parecer Financeiro' ELSE 'Parecer de Engenharia' END AS tipo,
									partexto,  
									to_char(pardata, 'DD/MM/YYYY') AS datadoparecer, 
									e.usunome AS nome
								FROM cte.parecerpar p
								INNER JOIN seguranca.usuario e ON e.usucpf = p.usucpf
								WHERE inuid = ".$inuid." 
									AND tppid in (1,2)
									AND date_part('year', p.pardata)= date_part('year', current_date) -- parecer do ano corrente	
								ORDER BY parhistorico DESC;";
		//dbg($sqlListaFinanceiro);
		//$arrayFinanceiro = $db->carregar($sqlListaFinanceiro);
		return $sqlListaFinanceiro;
	}
	
	function recuperavaloresgeraislista($subacao, $escola, $ano){
		global $db;
		
		if($escola == 't'){ // SQL quando a subação for por escola
			$select = "sum(cos.cosvlruni * ecs.ecsqtd) AS valor";
			$inner = "INNER JOIN cte.escolacomposicaosubacao ecs ON cos.cosid = ecs.cosid ";
			
		}else{ // SQL quando a subação for global
			$select = "sum(cos.cosqtd * cos.cosvlruni ) AS valor";
			$inner = "INNER JOIN cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND spt.sptano = ".$ano." --AND sptano = date_part('year', current_date)";
		}
		
		$sql=	"select ".$select."	
					from 
						cte.subacaoindicador sba
					INNER JOIN
						cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = ".$ano." --AND cosano = date_part('year', current_date)
					".$inner."
					where sba.sbaid = ".$subacao."
					group by sba.sbaid";
						
		return  $db->pegaUm( $sql );
	}
?>
