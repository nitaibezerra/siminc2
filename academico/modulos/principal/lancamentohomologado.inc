<?php

pegaSessoes(1, 1, 0);
verificaEditalPortaria($_SESSION["academico"]["edpid"], ACA_TPEDITAL_HOMOLOGACAO);

if ( !isset($_SESSION["academico"]["edpid"]) ){
	echo "<script>
			alert('É necessário cadastrar um edital primeiro!');
			window.location = '?modulo=principal/cadedital&acao=C';
		  </script>";
}

// instancia o objeto do módulo
$academico = new academico();

// Define as variáveis da tela
$prtid 			= $_SESSION['academico']['prtid'];
$entidcampus 	= $_SESSION['academico']['entidcampus'];
$entidentidade 	= $_SESSION['academico']['entid'];
$edpid 	 		= $_SESSION["academico"]["edpid"];
$prtid 	 		= $_SESSION["academico"]["prtid"];
$unidade 		= $_SESSION["academico"]["unidade"];
$ano   	 		= $_SESSION["academico"]["ano"];
$orgid 			= $_SESSION['academico']['orgid'];
$tpetipo 		= $_SESSION['academico']['tpetipo'];

//ações realizadas ao submeter a página
if($_REQUEST["submetido"] == 1) {
	
	// Deleta os registros
	$sql_delete  = "DELETE FROM academico.lancamentoeditalportaria WHERE edpid = ".$edpid;
	$db->executar($sql_delete);	
	
	// Insere os dados
	if($_REQUEST["crgid"] && $_REQUEST["crgid"][0] != "") {
		for($i=0; $i<count($_REQUEST["crgid"]); $i++) {	
					$sql_insert = "INSERT INTO academico.lancamentoeditalportaria
		                   			(edpid, crgid, lepvlrhomologado, lepano, lepdtinclusao)
		                			VALUES
		                   			(".$edpid.",".$_REQUEST["crgid"][$i].",".$_REQUEST["homologado"][$i].", '".$_SESSION['academico']['ano']."', now())";
					$db->executar($sql_insert);				
		}
	}
	$db->commit();
	echo "<script>
			alert('Dados salvos com sucesso.');
			window.location = '?modulo=principal/lancamentohomologado&acao=C';
		</script>";
	exit;
	
}

$parametros = array(
					'',
					'&edpid=' . $_SESSION['academico']['edpidhomo']
				  );
// monta o cabeçalho
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

//Monta as abas da tela
#$db->cria_aba($abacod_tela,$url,$parametros);

// cria o titulo da tela
$titulo_modulo = "Lançamento de Editais de Homologação";

monta_titulo( $titulo_modulo, "");

// mensagem de bloqueio
$bloqueado  = academico_mensagem_bloqueio( $_SESSION['academico']['orgid']);

$perfilNotBloq = array(
    PERFIL_IFESCADBOLSAS,
    PERFIL_IFESCADCURSOS,
    PERFIL_IFESCADASTRO,
    PERFIL_MECCADBOLSAS,
    PERFIL_MECCADCURSOS,
    PERFIL_MECCADASTRO,
    PERFIL_ADMINISTRADOR,
    PERFIL_INTERLOCUTOR_INSTITUTO,
    PERFIL_REITOR,
    PERFIL_PROREITOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança
$bloqueado = (!$bloqueado && $permissoes['gravar']) ? false : true;
$habil 		= $bloqueado ? 'N' : $habil;
$habilitado = $bloqueado ? false : $habilitado;

// Monta o Cabeçalho
if (isset($_SESSION["academico"]["edpid"])){ 
	$edpid=$_SESSION["academico"]["edpid"];
	$autoriazacaoconcursos = new autoriazacaoconcursos();
    $cabecalhoEdital = $autoriazacaoconcursos->cabecalhoedital($edpid, $prtid, $entidcampus );
}
?>
<style type="text/css" media="screen">
      /* Abas */
	.tituloaba{	
		width: 95%;
		margin:10px 0 0 0;
		left:2.5%;
		border-left:1px solid red;
		
	}
	
	.tituloaba2{
		font-size:12px; 
		text-align: center; 
		background-color: #dcdcdc; 
		height: 20px;
	}
       
</style>

<body>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script type="text/javascript" src="./geral/js/academico.js"></script>
    <div class='col-md-2' style='position:static;'>
        <?=cria_aba_2($abacod_tela, $url, $parametros);?>
    </div>
    <div class='col-md-9' style='position:static;'>
        <?= $cabecalhoEdital?>
        <form name="formulario" id="formulario" method="post" action="" >
            <input type="hidden" name="submetido" id="submetido" value="0">

            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" >
                <tr>
                    <td><br/></td>
                </tr>
                <?
                if($_SESSION['academico']['orgid'] != ACA_ORGAO_TECNICO):
                ?>
                <tr>
                    <td>
                        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center">
                            <tr>
                                <td class="tituloaba2">
                                    <b>Docentes do Magistério Superior</b>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                    <?php
                                        criarTabelaDocentesHomologado("tabela_docms", 1);
                                    ?>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td><br/></td>
                </tr>
                <?
                endif;
                ?>
                <tr>
                    <td>
                        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center">
                            <tr>
                                <td class="tituloaba2">
                                    <b>Professores da Educação Básica, Técnica e Tecnológica</b>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                <?php  criarTabelaDocentesHomologado("tabela_docente", 7); ?>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td><br/></td>
                </tr>
                <tr>
                    <td>
                        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center" >
                            <tr>
                                <td class="tituloaba2">
                                    <b>TA Nível E</b>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                <?php criarTabelaHomologado("tabela_tece", 2); ?>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td><br/></td>
                </tr>
                <tr>
                    <td>
                        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center" >
                            <tr>
                                <td class="tituloaba2">
                                    <b>TA Nível D</b>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                    <?php
                                        criarTabelaHomologado("tabela_tecd", 3);
                                    ?>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td><br/></td>
                </tr>
                <tr>
                    <td>
                        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center" >
                            <tr>
                                <td class="tituloaba2">
                                    <b>TA Nível C</b>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                    <?php
                                        criarTabelaHomologado("tabela_tecc", 4);
                                    ?>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td><br/></td>
                </tr>
                <tr>
                    <td>
                        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center" >
                            <tr>
                                <td class="tituloaba2">
                                    <b>TA Nível B</b>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                    <?php
                                        criarTabelaHomologado("tabela_tecb", 5);
                                    ?>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
                <?
                if($habilitado){
                    echo("
                        <tr>
                            <td><br/></td>
                        </tr>
                        <tr bgcolor='#C0C0C0'>
                            <td  align='left'>
                                <input type='button' id='salvar' value='Salvar' onclick='salvarDistribuicao();'>
                            </td>
                        </tr>");
                }
    ?>
            </table>
        </form>
    </div>
</body>
<script type="text/javascript"><!--

function salvarDistribuicao() {
	document.getElementById('submetido').value = 0;
	var lancado = document.getElementsByName("lancado[]");
	
	document.getElementById("salvar").disabled = true;
	
	var publicado  = document.getElementsByName("publicado[]");
	var homologado  = document.getElementsByName("homologado[]");	
		
	for (var i = 0; i < homologado.length; i++) {
		var elem = homologado[i];
		if((elem.value == '') && (elem.style.display != 'none')){
			alert('todos os campos devem ser peenchidos');
			elem.focus();
			document.getElementById("salvar").disabled = false;
			return;						
		}
	}	
	
	document.getElementById("submetido").value = 1;
	document.formulario.submit();
}

function excluirLinha2(index,tabela) {
	var tabela = document.getElementById(tabela);	
	tabela.deleteRow(index);
}

function valida_lancamento(nome_tabela, cargo_id){
	
	var id_disponivel_projetado  		= 'disponivel_projetado_'+nome_tabela;
	var id_td_disponivel_projetado 		= 'td_disponivel_projetado_'+nome_tabela;
	var id_utilizado_homologacao 		= 'utilizado_homologacao_'+nome_tabela;
	var id_td_utilizado_homologacao 	= 'td_utilizado_homologacao_'+nome_tabela;
	var id_disponivel_homologacao 		= 'disponivel_homologacao_'+nome_tabela;
	var id_td_disponivel_homologacao 	= 'td_disponivel_homologacao_'+nome_tabela;	
	var id_publicado  					= 'publicado_'+nome_tabela+'_'+cargo_id;
	var id_homologado 					= 'homologado_'+nome_tabela+'_'+cargo_id;
	
	var id_publicado_old  = 'publicado_old_'+nome_tabela+'_'+cargo_id;
	var id_homologado_old = 'homologado_old_'+nome_tabela+'_'+cargo_id;	

	var id_total_projetado  = 'total_pub_'+nome_tabela;
	var id_total_publicado  = 'total_pub_'+nome_tabela;
	var id_total_homologado = 'total_hom_'+nome_tabela;
	
	var publicado   		 	 	= document.getElementById(id_publicado);	
	var disponivel_projetado 	 	= document.getElementById(id_disponivel_projetado);
	var td_disponivel_projetado  	= document.getElementById(id_td_disponivel_projetado);	
	var utilizado_homologado 	 	= document.getElementById(id_utilizado_homologacao);
	var td_utilizado_homologado  	= document.getElementById(id_td_utilizado_homologacao);
	var disponivel_homologacao 	 	= document.getElementById(id_disponivel_homologacao);
	var td_disponivel_homologacao 	= document.getElementById(id_td_disponivel_homologacao);	
	var homologado  			 	= document.getElementById(id_homologado);
	var publicado_old   			= document.getElementById(id_publicado_old);
	var homologado_old  		 	= document.getElementById(id_homologado_old);
	
	var val_disponivel_projetado 	= parseInt(disponivel_projetado.value);
	var val_publicado   		 	= parseInt(publicado.value);
	var val_disponivel_homologacao 	= parseInt(disponivel_homologacao.value);
	var val_utilizado_homologado 	= parseInt(utilizado_homologado.value);
	var val_homologado  			= parseInt(homologado.value);
	var val_publicado_old  			= parseInt(publicado_old.value);
	var val_homologado_old  		= parseInt(homologado_old.value);	
	
	
	if( ( (val_homologado > (val_homologado_old + val_disponivel_homologacao) ) && (val_homologado != 0) ) 
			|| (val_homologado > val_publicado_old) 
			|| (isNaN(val_homologado)) ){
		
		alert('O Valor homologado dever ser menor ou igual ao Concurso Publicador e menor que o Disponível para Homologação.');
		homologado.value = val_homologado_old;		
		homologado.focus();
		homologado.select();
		calculaTotal(homologado, id_total_homologado);
		return;	
					
	}else if((val_homologado > 0) && (val_publicado_old == 0)){
	
		alert('É necessário cadastrar ao menos um Concurso Publicado.');
		homologado.value = val_homologado_old;
		homologado.focus();
		homologado.select();
		calculaTotal(homologado, id_total_homologado);		
		return;
	
	}else{
		
		homologado_old.value = val_homologado;
		utilizado_homologado.value = (val_utilizado_homologado + (val_homologado -  val_homologado_old));
		td_utilizado_homologado.innerHTML = (val_utilizado_homologado + (val_homologado -  val_homologado_old));		
		disponivel_homologacao.value = (val_disponivel_homologacao - (val_homologado -  val_homologado_old));	
		td_disponivel_homologacao.innerHTML = (val_disponivel_homologacao - (val_homologado -  val_homologado_old));
		
	}		
			
	calculaTotal(homologado, id_total_homologado);		
		
}


function valida_lancamento_docentes(nome_tabela, cargo_id){
	
	var id_disponivel_projetado  		= 'disponivel_projetado_'+nome_tabela;
	var id_td_disponivel_projetado 		= 'td_disponivel_projetado_'+nome_tabela;
	var id_utilizado_homologacao 		= 'utilizado_homologacao_'+nome_tabela;
	var id_td_utilizado_homologacao 	= 'td_utilizado_homologacao_'+nome_tabela;
	var id_disponivel_homologacao 		= 'disponivel_homologacao_'+nome_tabela;
	var id_td_disponivel_homologacao 	= 'td_disponivel_homologacao_'+nome_tabela;	
	var id_publicado  					= 'publicado_'+nome_tabela+'_'+cargo_id;
	var id_homologado 					= 'homologado_'+nome_tabela+'_'+cargo_id;
	
	var id_publicado_old  = 'publicado_old_'+nome_tabela+'_'+cargo_id;
	var id_homologado_old = 'homologado_old_'+nome_tabela+'_'+cargo_id;	

	var id_total_projetado  = 'total_pub_'+nome_tabela;
	var id_total_publicado  = 'total_pub_'+nome_tabela;
	var id_total_homologado = 'total_hom_'+nome_tabela;
	
	var publicado   		 	 	= document.getElementById(id_publicado);	
	var disponivel_projetado 	 	= document.getElementById(id_disponivel_projetado);
	var td_disponivel_projetado  	= document.getElementById(id_td_disponivel_projetado);	
	var utilizado_homologado 	 	= document.getElementById(id_utilizado_homologacao);
	var td_utilizado_homologado  	= document.getElementById(id_td_utilizado_homologacao);
	var disponivel_homologacao 	 	= document.getElementById(id_disponivel_homologacao);
	var td_disponivel_homologacao 	= document.getElementById(id_td_disponivel_homologacao);	
	var homologado  			 	= document.getElementById(id_homologado);
	var publicado_old   			= document.getElementById(id_publicado_old);
	var homologado_old  		 	= document.getElementById(id_homologado_old);
	
	var val_disponivel_projetado 	= parseInt(disponivel_projetado.value);
	var val_publicado   		 	= parseInt(publicado.value);
	var val_disponivel_homologacao 	= parseInt(disponivel_homologacao.value);
	var val_utilizado_homologado 	= parseInt(utilizado_homologado.value);
	var val_homologado  			= parseInt(homologado.value);
	var val_publicado_old  			= parseInt(publicado_old.value);
	var val_homologado_old  		= parseInt(homologado_old.value);	
	
	
	if( ( (val_homologado > (val_homologado_old + val_disponivel_homologacao) ) && (val_homologado != 0) ) 
			|| (val_homologado > val_publicado_old) 
			|| (isNaN(val_homologado) ) ){
		
		alert('O Valor homologado dever ser menor ou igual ao Concurso Publicador e menor que o Disponível para Homologação.');
		homologado.value = val_homologado_old;		
		homologado.focus();
		homologado.select();		
		return;	
					
	}else if((val_homologado > 0) && (val_publicado_old == 0)){
	
		alert('É necessário cadastrar ao menos um Concurso Publicado.');
		homologado.value = val_homologado_old;
		homologado.focus();
		homologado.select();		
		return;
	
	}else{
		
		homologado_old.value = val_homologado;
		utilizado_homologado.value = (val_utilizado_homologado + (val_homologado -  val_homologado_old));
		td_utilizado_homologado.innerHTML = (val_utilizado_homologado + (val_homologado -  val_homologado_old));		
		disponivel_homologacao.value = (val_disponivel_homologacao - (val_homologado -  val_homologado_old));	
		td_disponivel_homologacao.innerHTML = (val_disponivel_homologacao - (val_homologado -  val_homologado_old));
		
	}	
}

function valida_lancamento_docentes2(nome_tabela, cargo_id){

	var id_disponivel_projetado    = 'disponivel_projetado_'+nome_tabela;
	var id_td_disponivel_projetado 	= 'td_disponivel_projetado_'+nome_tabela;
	
	var id_projetado  	  = 'projetado_'+nome_tabela;
	var id_publicado  	  = 'publicado_'+nome_tabela+'_'+cargo_id;
	var id_homologado 	  = 'homologado_'+nome_tabela+'_'+cargo_id;
	
	var id_publicado_old  = 'publicado_old_'+nome_tabela+'_'+cargo_id;
	var id_homologado_old = 'homologado_old_'+nome_tabela+'_'+cargo_id;
	
		
	var projetado   = document.getElementById(id_projetado);
	var publicado   = document.getElementById(id_publicado);
	
	if(document.getElementById(id_publicado_old)){
		var val_publicado_old   = parseInt(document.getElementById(id_publicado_old).value);
	}else{
		var val_publicado_old = 0;
	}

	if(document.getElementById(id_homologado_old)){
		var val_homologado_old   = parseInt(document.getElementById(id_homologado_old).value);
	}else{
		var val_homologado_old = 0;
	}
			
	var disponivel_projetado 	= document.getElementById(id_disponivel_projetado);	
	var td_disponivel_projetado = document.getElementById(id_td_disponivel_projetado);
		
	var val_projetado   = parseInt(projetado.value);
	var val_publicado   = parseInt(publicado.value);
	var val_disponivel_projetado = parseInt(disponivel_projetado.value);
		
	
	if(document.getElementById(id_homologado).style.display != 'none' ){
	
		var homologado  	= document.getElementById(id_homologado);
		var val_homologado  = parseInt(homologado.value);		
		
		if( ((val_homologado > val_publicado_old)  && val_homologado != 0 ) 
				|| (isNaN(val_homologado)) ){
			alert('O Valor homologado dever ser menor ou igual ao Concurso Publicador.');
			homologado.value = val_homologado_old;
			homologado.focus();
			homologado.select();				
			return;				
		}
		
		if((val_homologado > 0) && (val_publicado_old == 0)){
		
			alert('É necessário cadastrar ao menos um Concurso Publicado.');
			homologado.value = val_homologado_old;
			homologado.focus();
			homologado.select();			
			return;			
		}			
	}
	
}
--></script>

