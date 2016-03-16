<?php
require_once APPRAIZ . 'includes/classes/consultaReceitaFederal.class.inc';
if($_POST['requisicao'] == "RFsalvar"){
	$RF = new ConsultaReceitaFederal("J");
	$insid = $RF->salvarDados();
	$_SESSION['snf']['insid'] = $insid;
	
	if($_POST['dircpf']){
		salvarDirigenteMax($insid);
	}
	
	header("Location: snf.php?modulo=principal/cadastrarInstituicao&acao=A");
	exit;
}
if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
}

include  APPRAIZ."includes/cabecalho.inc";

?>
<br />
<?php $db->cria_aba($abacod_tela,$url,''); ?>
<?php monta_titulo( 'Dados da Instituição de Ensino' , obrigatorio()." Indica campos obrigatórios.");  ?>

<link href="../snf/geral/snf.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/funcoes.js" ></script>
<script type="text/javascript" src="../includes/entidadesn.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>		
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript">
	jQuery.noConflict();
	jQuery(function() {
		
		jQuery("[name='cnpj']").change(function() {
			if(jQuery("[name='cnpj']").val()){
				var cnpj = jQuery("[name='cnpj']").val();
				jQuery.ajax({
					   type: "POST",
					   url: 'snf.php?modulo=principal/cadastrarInstituicao&acao=A',
					   data: "requisicao=recuperaDiretorPorPorCNPJ&cnpj=" + cnpj,
					   dataType: "xml",
					   success: function(xml){
					   		if(jQuery(xml).find('SEMDADOS').length <= 0){
						   		 jQuery(xml).find('DADOS').each(function () {
			                        jQuery("[name='dircpf']").val(mascaraglobal("###.###.###-##",jQuery(this).find('dircpf').text()));
	                        		jQuery("[name='dirnome']").val(jQuery(this).find('dirnome').text());
	                        		jQuery("[name='carid']").val(jQuery(this).find('carid').text());
			                    });
			                 }
					   }
					 });
			}
		});
		
	});
	
	function verificaCPFRF(obj)
	{
		if(obj.value){
			jQuery.ajax({
					   type: "POST",
					   url: 'snf.php?modulo=principal/cadastrarInstituicao&acao=A',
					   data: "requisicao=verificaCPFRF&cpf=" + obj.value,
					   success: function(msg){
					   		if(msg.search("Nome:") >= 0){
					   			jQuery("#dirnome").val(msg.replace("Nome:",""));
					   		}else{
					   			alert(msg);
					   			jQuery("#dirnome" ).val("");
					   		}
					   }
					 });
		}else{
			jQuery("#dirnome").val("");
		}
	}
        
</script>

<?php
$RF = new ConsultaReceitaFederal("J");
$RF->schema("snf");
$RF->table("instituicaoensino");
$RF->enderecoSchema("snf");
$RF->enderecoTable("endereco");
if($_GET['insid']){
	$RF->carregarDados($_GET['insid']);
	$arrDados = recuperaDadosDirigente($_GET['insid']);
}
$camposForm[] = array( 
						"titulo" => "Dados Da Instituição" , 
						"funcao" => "titulo",
					   );
$camposForm[] = array( 
						"nome" => "insnome" , 
						"descricao" => "Instituição" , 
						"funcao" => "campo_texto" , 
						"obrigatorio" => true,
						"habilitado" => false, 
						"receita" => "no_empresarial_rf",
						"tamanho" => "80"
					   );
$camposForm[] = array( 
						"nome" => "inssigla" , 
						"descricao" => "Sigla" , 
						"funcao" => "campo_texto" , 
						"obrigatorio" => false,
						"habilitado" => true, 
						"receita" => "entsigla",
						"tamanho" => "20"
					   );
$camposForm[] = array( 
						"nome" => "instelefone" , 
						"descricao" => "Telefone" , 
						"funcao" => "campo_texto" , 
						"obrigatorio" => true,
						"habilitado" => true,
						"tamanho" => "20"
					   );
$camposForm[] = array( 
						"nome" => "insfax" , 
						"descricao" => "Fax" , 
						"funcao" => "campo_texto" , 
						"obrigatorio" => true,
						"habilitado" => true,
						"tamanho" => "20"
					   );
$camposForm[] = array( 
						"nome" => "insemail" , 
						"descricao" => "Email" , 
						"funcao" => "campo_texto" , 
						"obrigatorio" => false,
						"habilitado" => true,
						"tamanho" => "60",
						"validacao" => "email"
					   );
$camposForm[] = array( 
						"nome" => "natid" , 
						"descricao" => "Natureza Jurídica" , 
						"funcao" => "monta_combo" , 
						"obrigatorio" => true,
						"habilitado" => true,
						"campoCombo" => "natdescricao",
						"where" =>"natstatus = 'A' and natid in (4,5,6)"
					   );
$camposForm[] = array( 
						"nome" => "insstatus" , 
						"funcao" => "hidden" , 
						"value" => "A"
					   );
$camposForm[] = array( 
						"nome" => "endid" , 
						"funcao" => "monta_endereco",
						"schema" => "snf",
						"tabela" => "endereco"
					   );
$html = "<tr><td class='SubtituloTabela bold' colspan='2' >Dados do Dirigente</td></tr>";
$html.= "<tr><td class='SubtituloDireita' >CPF:</td><td>".campo_texto("dircpf","S","S","",18,14,"###.###.###-##","","","",""," id='dircpf' onchange='verificaCPFRF(this)' ","", ($arrDados['dircpf'] ? mascaraglobalTermoAdesao($arrDados['dircpf'],"###.###.###-##") : "") )."</td></tr>";
$html.= "<tr><td class='SubtituloDireita' >Nome:</td><td>".campo_texto("dirnome","S","N","",60,255,"","","","",""," id='dirnome' ","",$arrDados['dirnome'])."</td></tr>";
$sqlCargo = "	select 
										carid as codigo, 
										cardescricao as descricao 
									from 
										snf.cargo 
									where 
										carstatus = 'A'
									order by 
										cardescricao";
$html.= "<tr><td class='SubtituloDireita' >Cargo:</td><td>".$db->monta_combo("carid",$sqlCargo,"S","Selecione...","","","",200,"S","",true,$arrDados['carid'])."</td></tr>";
$camposForm[] = array( 
						"funcao" => "monta_html",
						"html" => $html, 
					   );
$RF->camposFormulario($camposForm);
$RF->exibirFormulario();
?>

