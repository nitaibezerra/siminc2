<?

//Chamada de programa
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$academico = new academico();
?>
<script>
    function acessaPagina(url){
        window.location = url;
    }
</script>
<style>
    .acima:hover{
        background-color: #ffffcc;
    }
</style>
<?php
function retornaAbas()
{
    global $db;
    $sql = "
        SELECT
            menu.mnuid,
            menu.mnudsc,
            menu.mnulink,
            menu.mnutransacao
        FROM seguranca.menu, seguranca.aba_menu
        WHERE menu.mnuid = aba_menu.mnuid
            AND aba_menu.abacod = 57135
            AND menu.sisid = {$_SESSION['sisid']}
            AND menu.mnuid IN (
                SELECT DISTINCT m2.mnuid FROM seguranca.perfilmenu m2, seguranca.perfilusuario p
                WHERE m2.pflcod = p.pflcod AND p.usucpf = '{$_SESSION['usucpf']}' AND mnuid <> 13355
               )
        ORDER BY menu.mnudsc";
    $resultSet = $db->carregar($sql);
    return $resultSet;
}

if ($_REQUEST['carga']) {
	$_SESSION['academico']['orgid'] = $_SESSION['academico']['orgid'] ? $_SESSION['academico']['orgid'] : $_REQUEST["orgid"];
    $abas = retornaAbas();
    $saida = "";
    if(is_array($abas)){
        foreach($abas as $key => $aba){
            $bg = 'white';
            if($key % 2 ==  1){
                $bg = '#f9f9f9';
            }
            $transacao = str_replace(' ' , '' , $aba['mnutransacao']);
            $saida .= <<<HTML
                <tr bgcolor onmouseover="this.bgColor='#cce0ff';" onmouseout="this.bgColor='$bg';" style="cursor:pointer;" onclick="acessaPagina('{$aba['mnulink']}&entidunidade={$_REQUEST["carga"]}');" style="">
                    <td valign="top" style="font-size:12px; border:1px solid #ccc; padding: 2px 0; width: 30px;" class="text-center">
                        <span style="color:#6495ED;" class="glyphicon glyphicon-hand-right"></span>
                    </td>
                    <td valign="top" class="title" style="font-size:12px; padding: 2px 0 5px 5px; border:1px solid #ccc;"> {$aba['mnudsc']} </td>
                </tr>
HTML;
        }
    }
?>
<div class="col-md-8" style="padding:10px; width: 97%; border:1px solid; color:#D3D3D3; box-shadow:2px 0px 3px #696969;">
    <div class="col-md-3" style="position:static;">
        <table style='width: 100%;' border="0" cellspacing="0" cellpadding="0">
            <tr>
                <th colspan='2'>Abas</th>
            </tr>                    
            <? echo $saida == '' ? '' : $saida ?>
        </table>
    </div>
    <div class='col-md-8' style='position:static;'>
        <table style='width: 100%;'>
            <tr>
                <th>Lista de Campus</th>
            </tr>
            <tr>
                <td><?$academico->academico_listacampus( $_REQUEST["carga"], $_SESSION['academico']['orgid']);?></td>
            </tr>
        </table>
		<?
		if ( !verificaPerfil( PERFIL_DIRETOR_CAMPUS ) ) {
			?>
			<table style='width: 100%;'>
				<tr>
					<th>Reitoria</th>
				</tr>
				<tr>
					<td><?$academico->academico_listareitoria($_REQUEST["carga"], $_SESSION['academico']['orgid']); ?></td>
				</tr>
			</table>
		<?
		}
		?>
    </div>  
</div>
<?PHP
    die;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$orgid = academico_existeorgao($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION['academico']['orgid']; 

//if ( empty( $_GET['orgid'] ) ){
//	$_REQUEST['orgid'] = $_SESSION['academico']["orgid"];
//}

// limpa todas as sessões do módulo
unset($_SESSION['academico']);

if( $_REQUEST['atualizaCampus'] ){
	$entidNovo = $_REQUEST['entidNovo'];
	$sql = "UPDATE entidade.entidade SET entstatus= 'I' WHERE entid ={$entidNovo}"; 
	$db->executar($sql);
	$db->commit();
}
if( $_REQUEST['atualizaReitorias'] ){
	$entidID = $_REQUEST['entidID'];
	$sql = "UPDATE entidade.entidade SET entstatus= 'I' WHERE entid ={$entidID}"; 
	$db->executar($sql);
	$db->commit();
}

// Monta as abas com os orgão de responsabilidade do usuário (se nao for superUser)
$res = academico_pega_orgao_permitido();

if ($res[0]["id"] != ''){
	$orgaoPermitido = false;
	foreach($res as $dadosOrg){
		if ($dadosOrg['id'] == $orgid && $orgid != ''){
			$orgaoPermitido = true;
			continue;
		}
	}
	if ( !$orgaoPermitido ){
		$orgid = $res[0]['id'];
	}
	
//	$org = $_REQUEST['orgid'] ? $_REQUEST['orgid'] : $res[0]['id'];
	$link = '/academico/academico.php?modulo=inicio&acao=C&orgid=' . $orgid;
	$_SESSION['academico']['orgid'] = $orgid;
	
	echo montarAbasArray($res, $link );
	monta_titulo( 'Lista de Instituições', '' );

$arrPerfil = pegaPerfilGeral();

if( 1==2 && (in_array(PERFIL_SUPERUSUARIO,$arrPerfil) || in_array(PERFIL_ADMINISTRADOR,$arrPerfil) || in_array(PERFIL_IFESCADASTRO,$arrPerfil) || in_array(PERFIL_MECCADASTRO,$arrPerfil) || in_array(PERFIL_CADASTROGERAL,$arrPerfil)) ) {
	/*
	$texto = "<div style=\"font-size:12px\" >Senhores Usuários,<br /><br />
	Informamos que foi implementada no Sistema Integrado de Monitoramento Execução e Controle - SIMEC, no Módulo Rede Federal, a funcionalidade <b>\"Solicitação Viagem Exterior\"</b> destinada a agilizar o processo de Autorização Coletiva para concessão de diárias e passagens ao exterior pelo Ministro de Estado da Educação, desde que referidos aos programas de treinamento, capacitação, qualificação, intercâmbio acadêmico, cooperação internacional, pós-graduação e inovação, mediante a aprovação dos conselhos superiores dos respectivos órgão/entidades e a especificação do número de participantes, conforme definido no artigo 4º, § 1º, da Portaria MEC nº. 446, de 20 de abril de 2011, deverão seguir os seguintes passos: <a target=\"_blank\" href=\"manualsolicitacaoviagem.pdf\" >clique aqui</a>.</div>";
	*/
	
	/*
	$texto = "<div style=\"font-size:12px\" >Senhores Usuários,<br /><br />
	Informamos que está disponível no Sistema Integrado de Monitoramento Execução e Controle - SIMEC, no Módulo Rede Federal, a funcionalidade <b>“Solicitação Viagem Exterior”</b>, visando o cumprimento do estabelecido pelo Decreto 7.689 de 02/03/2012, Portaria MPOG nº 75 de 08/03/2012, Portaria MEC nº 362 de 10/04/2012 e Portaria MEC nº 441 de 25/04/2012.
	<br><br>
	Para acessar obter maiores informações relacionadas aos procedimentos para utilização do sistema: <a target=\"_blank\" href=\"manualsolicitacaoviagem.pdf\" >clique aqui</a>.</div>";
	*/
	
	$texto = "<div style=\"font-size:12px\" >Caros reitores das Instituições Federais de Ensino Superior e dirigentes de escolas médicas ao nível federal (diretores ou coordenadores de curso):<br /><br />
	<p>Inicia-se no dia 11 de julho de 2013 a manifestação de interesse das IFES em serem partícipes do Programa Mais Médicos para o Brasil, anunciado pela presidenta Dilma Roussef, pelo ministro da Educação Aloizio Mercadante e pelo ministro da Saúde Alexandre Padilha no dia 08 de julho de 2013.</p> 
	<p>Este programa tem como finalidade de garantir o provimento de médicos para áreas de difícil fixação e profissionais, assim como garantir segundo ciclo de formação médica para se avançar na construção do perfil de egresso estabelecido pelas Diretrizes Curriculares Nacionais (DCN’s) dos cursos de graduação em Medicina, na perspectiva de formação de um médico generalista, crítico, reflexivo e humanista.</p>
	<p>Dentro deste conjunto de medidas, há ações interministeriais entre Ministério da Educação e Ministério da Saúde que estabelecem políticas voltadas para expansão de vagas para residência médica e investimentos na rede de serviços de saúde. </p>
	<p>O apoio das IFES será fundamental para avançar na construção das mudanças na formação implicadas com a definição do segundo ciclo de formação, bem como apoiar, na forma de tutoria e preceptoria, as atividades de supervisão direta estabelecidas com os profissionais dos serviços de saúde.</p> 
	<p>Para manifestação e interesse pela instituição, é necessário a assinatura de termo de pré adesão pelo reitor da instituição,  conforme previsto na Portaria Normativa nº 14, de 9 de julho de 2013, do GM/MEC. Este deverá ser inserido no SIMEC, em aba específica incluída dentro do módulo rede federal. O termo deverá ser digitalizado e inserido no sistema, bem como enviado cópia impressa, via SEDEX com Aviso de Recebimento (AR), para o endereço da DIFES – Diretoria de Desenvolvimento da Rede de Instituições federais de Ensino Superior, Ministério da Educação – MEC, Edifício Sede, Bloco “L”, 3º Andar, Sala 303, Esplanada dos Ministérios, Brasília-DF. CEP: 70047-900.</p>
	<p>Lembramos que o prazo para pré-adesão será até às 23:59 hs do dia 25 de julho de 2013.</p>
	<p>Após este momento, haverá o prazo para que as IFES, caso necessário, realizem consultas a seus conselhos universitários para ratificação da adesão definitiva. Esta tem previsão de ocorrer, mediante assinatura de novo termo, entre os dias 10 a 13 de agosto de 2013.</p>
	<p>Quaisquer dúvidas no telefone (61) 2022-8158, ou pelo endereço eletrônico: rosa.nader@mec.gov.br</p>
	<p>Contamos com o apoio de todos para avançarmos na construção do direito à saúde no Brasil e na garantia das mudanças necessárias no âmbito da formação médica.</p>
	</div>";

	popupAlertaGeral($texto,'900px',"470px");
}

if(in_array(PERFIL_REITOR,$arrPerfil) && $orgid==1 && 1==2){
	$texto = "<div style=\"font-size:12px\" >Caros reitores das Instituições Federais de Ensino Superior e dirigentes de escolas médicas ao nível federal (diretores ou coordenadores de curso):<br /><br />
	<p>Inicia-se no dia 11 de julho de 2013 a manifestação de interesse das IFES em serem partícipes do Programa Mais Médicos para o Brasil, anunciado pela presidenta Dilma Roussef, pelo ministro da Educação Aloizio Mercadante e pelo ministro da Saúde Alexandre Padilha no dia 08 de julho de 2013.</p> 
	<p>Este programa tem como finalidade de garantir o provimento de médicos para áreas de difícil fixação e profissionais, assim como garantir segundo ciclo de formação médica para se avançar na construção do perfil de egresso estabelecido pelas Diretrizes Curriculares Nacionais (DCN’s) dos cursos de graduação em Medicina, na perspectiva de formação de um médico generalista, crítico, reflexivo e humanista.</p>
	<p>Dentro deste conjunto de medidas, há ações interministeriais entre Ministério da Educação e Ministério da Saúde que estabelecem políticas voltadas para expansão de vagas para residência médica e investimentos na rede de serviços de saúde. </p>
	<p>O apoio das IFES será fundamental para avançar na construção das mudanças na formação implicadas com a definição do segundo ciclo de formação, bem como apoiar, na forma de tutoria e preceptoria, as atividades de supervisão direta estabelecidas com os profissionais dos serviços de saúde.</p> 
	<p>Para manifestação e interesse pela instituição, é necessário a assinatura de termo de pré adesão pelo reitor da instituição,  conforme previsto na Portaria Normativa nº 14, de 9 de julho de 2013, do GM/MEC. Este deverá ser inserido no SIMEC, em aba específica incluída dentro do módulo rede federal. O termo deverá ser digitalizado e inserido no sistema, bem como enviado cópia impressa, via SEDEX com Aviso de Recebimento (AR), para o endereço da DIFES – Diretoria de Desenvolvimento da Rede de Instituições federais de Ensino Superior, Ministério da Educação – MEC, Edifício Sede, Bloco “L”, 3º Andar, Sala 303, Esplanada dos Ministérios, Brasília-DF. CEP: 70047-900.</p>
	<p>Lembramos que o prazo para pré-adesão será até às 23:59 hs do dia 25 de julho de 2013.</p>
	<p>Após este momento, haverá o prazo para que as IFES, caso necessário, realizem consultas a seus conselhos universitários para ratificação da adesão definitiva. Esta tem previsão de ocorrer, mediante assinatura de novo termo, entre os dias 10 a 13 de agosto de 2013.</p>
	<p>Quaisquer dúvidas no telefone (61) 2022-8158, ou pelo endereço eletrônico: rosa.nader@mec.gov.br</p>
	<p>Contamos com o apoio de todos para avançarmos na construção do direito à saúde no Brasil e na garantia das mudanças necessárias no âmbito da formação médica.</p>
	</div>";

	popupAlertaGeral($texto,'900px',"470px");
}

if( in_array(PERFIL_ASSESSORIA_ALTA_GESTAO,$arrPerfil) || in_array(PERFIL_ALTA_GESTAO,$arrPerfil) || in_array(PERFIL_SUPERUSUARIO,$arrPerfil) || in_array(PERFIL_CADASTROGERAL,$arrPerfil) || in_array(PERFIL_IFESCADBOLSAS,$arrPerfil) || in_array(PERFIL_IFESCADCURSOS,$arrPerfil)  || in_array(PERFIL_IFESCADASTRO,$arrPerfil) ) {
	
		switch ( $orgid ){
			case '1':
				$funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
			break;
			case '2':
				$funid = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";
			break;
			case '3':
				$funid = " in ('" . ACA_ID_UNIDADES_VINCULADAS . "')";
			break;
		}
	
	if ( in_array(PERFIL_ASSESSORIA_ALTA_GESTAO,$arrPerfil) || in_array(PERFIL_ALTA_GESTAO,$arrPerfil) || $db->testa_superuser() || academico_possui_perfil(PERFIL_ADMINISTRADOR) || academico_possui_perfil_sem_vinculo() || academico_possui_perfil(PERFIL_MECCADASTRO) || academico_possui_perfil_resp_tipo_ensino()){

			$sql = "SELECT
						e.entid as codigo,
						entsig || ' - ' || UPPER(entnome) as descricao
					FROM
						entidade.entidade e 
					INNER JOIN
						entidade.funcaoentidade ef ON ef.entid = e.entid
					LEFT JOIN 
						entidade.endereco ed ON e.entid = ed.entid
					WHERE
						e.entstatus = 'A' AND ef.funid ". $funid . "
					GROUP BY e.entid, e.entnome , e.entsig
					ORDER BY
						 e.entsig, e.entnome";
			
		}else{
			
			$sql = "SELECT
						e.entid as codigo,
						entsig || ' - ' || UPPER(entnome) as descricao
					FROM
						entidade.entidade e 
					INNER JOIN
						entidade.funcaoentidade ef ON ef.entid = e.entid
					LEFT JOIN 
						entidade.endereco ed ON e.entid = ed.entid
					INNER JOIN
						academico.usuarioresponsabilidade ur ON ur.entid = e.entid
																AND ur.rpustatus = 'A'
					WHERE
						e.entstatus = 'A' AND ef.funid ". $funid . " AND ur.usucpf = '{$_SESSION['usucpf']}'
					GROUP BY e.entid, e.entnome,  e.entsig
					ORDER BY
						 e.entsig, e.entnome";
			
		}
		
		$arrDados = $db->carregar($sql);
		
		/*
		if($arrDados){
			if(count($arrDados) > 1){
				$combo = $db->monta_combo("entid",$arrDados,"S","Selecione a Instituição...","exibeSolicitacaoDecreto","","","","","",true);
			}else{
				foreach($arrDados as $dado){
					$solicitacao = $dado['codigo'];
				}
			}
				
			$texto = "<center>
					<img src=\"../imagens/alerta_sistema.gif\" />
				    <p><font size=3 color=red><b>Atenção!</b></font></p>
	           	     Solicitação de excepcionalidade decreto 7446 de 1º de Março de 2011.<br />
	           	     Prazo para envio das solicitações: 13/04/2011.<br />
	           	     ".($combo ? "Selecione: ".$combo."<br />" : "<a href=\"academico.php?modulo=principal/solicitacaodecreto&acao=A&entid=$solicitacao\" >Clique aqui</a>");
		
			popupAlertaGeral($texto,"580px","260px");
		}
		*/
}
	
?>
	<form name="formulario" id="pesquisar" method="POST" action="">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
			<tr>
				<td  bgcolor="#CCCCCC" colspan="2"><b>Argumentos da Pesquisa</b></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Instituição</td>
				<td>
					<?php
					
						switch ( $orgid ){
							case '1':
								$funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
							break;
							case '2':
								$funid = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";
							break;
							case '3':
								$funid = " in ('" . ACA_ID_UNIDADES_VINCULADAS . "')";
							break;
						}
							
						$sql_unidade = "SELECT
											e.entid as codigo,
											e.entnome as descricao
										FROM
											entidade.entidade e
										INNER JOIN
											entidade.funcaoentidade ef ON ef.entid = e.entid
										WHERE
											e.entstatus = 'A' AND ef.funid {$funid}
										ORDER BY
											e.entnome";
						$entid = $_REQUEST['entid'] ? $_REQUEST['entid'] : ''; 
						
						$db->monta_combo("entid", $sql_unidade, "S", "Todas as Instituições", '', '', '', '', 'N','entid');
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">UF</td>
				<td>
					<?php
						
						$sql_uf = "SELECT
									estuf as codigo,
									estdescricao as descricao
								FROM
									territorios.estado";
						
						$estuf = $_REQUEST['estuf'] ? $_REQUEST['estuf'] : '';
						
						$db->monta_combo("estuf", $sql_uf, "S", "Todas", '', '', '', '150', 'N','estuf');
						
					?>
				</td>
			</tr>
			<tr>
				<td bgcolor="#CCCCCC"></td>
				<td bgcolor="#CCCCCC">
					<input style="cursor:pointer;" type="submit" value="Pesquisar" />
					<input style="cursor:pointer;" type="button" value="Ver Todos" onCLick="academico_verTodasUnidades();" />
				</td>
			</tr>
		</table>
	</form>
<?php

	// lista as unidades
	$academico->academico_listaunidades( $orgid );

}else{ ?>
	
	<?php monta_titulo( 'Lista de Instituições', '' ); ?>
	<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
		<tr style="text-align: center; color: #ff0000;">
			<td>
				Usuário sem permissões para visualizar unidades. <br/>
				Favor entrar em contato com o gestor do módulo para verificar as responsabilidades de seu perfil.
			</td>
		</tr>
	</table>

<?php } ?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script>
	function academico_abreSistema( entid ){
		window.location.href = '/academico/academico.php?modulo=principal/dadosentidade&acao=A&entidunidade=' + entid;
	}
	
	function academico_verTodasUnidades(){
		var form = window.document.getElementById("pesquisar");
		
		for( var i=0; i < form.elements.length; i++ ){
			var campo = form.elements[i];
			
			switch(campo.type){
				case "select-one":
					campo.options[0].selected = true;
			}
		}
		
		form.submit();
		
	}

	var params;
	function formatarParametros(){
    	params = Form.serialize($('pesquisar'));
	}

	function desabilitarConteudo( id ){
		var url = 'academico.php?modulo=inicio&acao=A&carga='+id;
		if ( document.getElementById('img'+id).name == '-' ) {
			url = url + '&subAcao=retirarCarga';
			var myAjax = new Ajax.Request(
				url,
				{
					method: 'post',
					asynchronous: false
				});
		}
	}
	function abredadoscampus( entid ){
		return window.location = '?modulo=principal/alterarCampus&acao=A&entid=' + entid;
	}
	function abredadosreitoria( entid ){
		return window.location = '?modulo=principal/alterarReitoria&acao=A&entid=' + entid;
	}

	
	function inativarcampus( entidade ){
		if(confirm("Deseja realmente excluir o Campus"))
		  {
			var myAjax = new Ajax.Request('academico.php?modulo=inicio&acao=C', {
		        method:     'post',
		        parameters:  '&atualizaCampus=true&entidNovo=' + entidade,
		        onComplete: function (res){	
	       			alert("Campus excluido com sucesso!");
	       			window.location.href = window.location.href;
		        }
		  	}); 
		  }
	}
	function inativarreitoria( entidID ){
		if(confirm("Deseja realmente excluir a Reitoria"))
		  {
			var myAjax = new Ajax.Request('academico.php?modulo=inicio&acao=C', {
		        method:     'post',
		        parameters:  '&atualizaReitorias=true&entidID=' + entidID,
		        onComplete: function (res){	
	       			alert("Reitoria excluida com sucesso!");
	       			window.location.href = window.location.href;
		        }
		  	}); 
		  }
	}
	
	function exibeSolicitacaoDecreto(entid)
	{
		window.location = 'academico.php?modulo=principal/solicitacaodecreto&acao=A&entid=' + entid;
	}
	
	function aviso(){
		alert("O sistema abre no dia 02 de julho de 2012.");
	}
</script>