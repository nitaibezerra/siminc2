
<script language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/entidades.js"></script>
<script language="JavaScript" src="./geral/js/academico.js"></script>

<?php

if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
}

$orgid = $_SESSION['academico']['orgid'];

$perfilNotBloq = array(
    PERFIL_MECCADBOLSAS,
	PERFIL_MECCADCURSOS,
	PERFIL_MECCADASTRO,
	PERFIL_ADMINISTRADOR,
    PERFIL_REITOR,
    PERFIL_INTERLOCUTOR_INSTITUTO,
    PERFIL_INTERLOCUTOR_CAMPUS,
    PERFIL_DIRETOR_CAMPUS,
    PERFIL_PROREITOR
);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$orgid);


// cabecalho pradrão do SIMEC
include_once APPRAIZ . "includes/cabecalho.inc";

print "<br/>";
monta_titulo('Processo Seletivo','');
?>
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
<?php
$autoriazacaoconcursos = new autoriazacaoconcursos();
#$db->cria_aba($abacod_tela,$url,$parametros);
echo $autoriazacaoconcursos->cabecalho_entidade($_SESSION['sig_var']['entid']);
#$bloqueado = monta_cabecalho_academico($_SESSION['sig_var']['entid'], "processo");

echo "<form name='formulario' id='formulario' method='post' action='?modulo=principal/processoSeletivo&acao=A&exec_function=salvarProcessoSeletivo'>";
			if($_REQUEST['prsid']) {
				$processoseletivo = $db->carregar("SELECT * FROM academico.processoseletivo WHERE prsid='".$_REQUEST['prsid']."'");
				if($processoseletivo[0]) {
					$processoseletivo=current($processoseletivo);
					$prsinscricaoini = $processoseletivo['prsinscricaoini'];
					$prsinscricaofim = $processoseletivo['prsinscricaofim'];
					$prsprovaini = $processoseletivo['prsprovaini'];
					$prsprovafim = $processoseletivo['prsprovafim'];
					$prsinicioaula = $processoseletivo['prsinicioaula'];
					$prsnrvagas = $processoseletivo['prsnrvagas'];
					$prsobservacao = $processoseletivo['prsobservacao'];
					echo "<input type='hidden' name='prsid' value='".$processoseletivo['prsid']."'>";
				}
			}
			echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
			echo "<tr><td colspan='2' class='subtitulocentro' height='30px'><strong>Cadastrar processo seletivo de estudantes</strong></td></tr>";
			echo "<tr><td class=\"SubTituloDireita\">Inscrições :</td><td>de ".campo_data( 'prsinscricaoini', 'N', 'S', '', 'S' )." até ".campo_data( 'prsinscricaofim', 'N', 'S', '', 'S' )."</td>";
			echo "<tr><td class=\"SubTituloDireita\">Provas :</td><td>de ".campo_data( 'prsprovaini', 'N', 'S', '', 'S' )." até ".campo_data( 'prsprovafim', 'N', 'S', '', 'S' )."</td>";
			echo "<tr><td class=\"SubTituloDireita\">Data de início das aulas :</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".campo_data( 'prsinicioaula', 'N', 'S', '', 'S' )."</td>";
			echo "<tr><td class=\"SubTituloDireita\">Número de vagas ofertadas :</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;". campo_texto('prsnrvagas', "N", "S", "Número de vagas ofertadas", 12, 10, "##########", "", '', '', 0, '' )."</td>";
			echo "<tr><td class=\"SubTituloDireita\">Observação :</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;". campo_textarea('prsobservacao', "N", "S", "Observação", 50, 6, "300", "", '', '', 0, '' )."</td>";
			if ($permissoes['gravar']){
				echo "<tr><td bgcolor=\"#C0C0C0\" colspan='2' align='center'><input type='button' onclick=\"return validaprocessoseletivo();this.disabled=true;\" value='Salvar processo seletivo' ></td>";
			}else{
				echo "<tr><td bgcolor=\"#C0C0C0\" colspan='2' align='center'><input type='button' disabled='disabled' value='Salvar processo seletivo' ></td>";
			}	
			echo "</table>";
			echo "</form>";
			
			/*<input type='button' value='Novo' onclick=\"window.location='?modulo=principal/inserir_entidade&acao={$acao}&page=esp';this.disabled=true;\">*/
			
			$sql = "SELECT 
						'<center>
							<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/processoSeletivo&acao=A&page=esp&prsid='|| prsid ||'\'\"> 
							<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=principal/processoSeletivo&acao=A&exec_function=salvarProcessoSeletivo&rmprsid='|| prsid ||'\',\'Deseja realmente excluir este processo seletivo?\');\">
						 </center>', 
						'de '||(to_char(prsinscricaoini,'DD/MM/YYYY') ||' até '|| to_char(prsinscricaofim,'DD/MM/YYYY')) as perinsc, 'de '||(to_char(prsprovaini,'DD/MM/YYYY') ||' até '|| to_char(prsprovafim,'DD/MM/YYYY')) as perprova, 
						to_char(prsinicioaula,'DD/MM/YYYY'), 
						prsnrvagas,
						prsobservacao 
					FROM 
						academico.campus cam
				    INNER JOIN 
				    	academico.processoseletivo prs ON prs.cmpid = cam.cmpid  
					WHERE 
						cam.entid='".$_SESSION['sig_var']['entid']."'";

			$cabecalho = array("","Inscrição","Provas","Início das aulas","Número de vagas","Observação");
			echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
			echo "<tr><td class=\"SubTituloCentro\" colspan='4'><strong>Lista de processos seletivos</strong></td></tr>";
			echo "<tr><td>";
			$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
			echo "</td></tr>";
			echo "</table>";

?>
</div>