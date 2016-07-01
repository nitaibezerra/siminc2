<?

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';
// abre conexão com o servidor de banco de dados
$db = new cls_banco();


/* PADRONIZANDO A PASTA DOCUMENTOS */

// pega o endereço do diretório
$diretorio = '../../arquivos/obras/documentos/'; 
// abre o diretório
$ponteiro  = opendir($diretorio);
// monta os vetores com os itens encontrados na pasta
while ($nome_itens = readdir($ponteiro)) {
	if($nome_itens != '.' && $nome_itens != '..' && $nome_itens != '.svn') {
		if(!is_dir('../../arquivos/obras/'.floor($arqid/1000))) {
			$conf = mkdir(APPRAIZ.'/arquivos/obras/'.floor($arqid/1000), 0777);
			if(!$conf) {
				echo "Não foi possível criar a pasta. Verifique com o administrador.";
				exit;	
			}
		}
		if(@copy("../../arquivos/obras/documentos/".$nome_itens,"../../arquivos/obras/".floor($nome_itens/1000)."/".$nome_itens)){
			echo "Arquivo N: ".$nome_itens." transferido com sucesso.<br />";
		} else {
			echo "Problemas na cópia do arquivo N: ".$nome_itens.".<br />";
			exit;
		}
	}
}
/* PADRONIZANDO A PASTA DOCUMENTOS */


/* PADRONIZANDO A PASTA IMGS */

// pega o endereço do diretório
$diretorio = '../../arquivos/obras/imgs/'; 
// abre o diretório
$ponteiro  = opendir($diretorio);
// monta os vetores com os itens encontrados na pasta
while ($nome_itens = readdir($ponteiro)) {
	
	if($nome_itens != '.' && $nome_itens != '..' && $nome_itens != '.svn') {
		$sql = "SELECT arq.arqid FROM public.arquivo AS arq  
			WHERE arqnome = '". $nome_itens ."' AND arqtipo = 'image'"; 
		$arquivo = current($db->carregar($sql));
	
		if(!is_dir('../../arquivos/obras/'.floor($arquivo['arqid']/1000))) {
			$conf = mkdir(APPRAIZ.'/arquivos/obras/'.floor($arquivo['arqid']/1000), 0777);
			if(!$conf) {
				echo "Não foi possível criar a pasta. Verifique com o administrador.";
				exit;	
			}
		}
		if(@copy("../../arquivos/obras/documentos/".$arquivo['arqid'],"../../arquivos/obras/".floor($arquivo['arqid']/1000)."/".$arquivo['arqid'])){
			echo "Arquivo N: ". $arquivo['arqid'] ." transferido com sucesso.<br />";
		} else {
			echo "Problemas na cópia do arquivo N: ". $arquivo['arqid'] .".<br />";
			exit;
		}
		
	}
}

/* PADRONIZANDO A PASTA IMGS */

?>